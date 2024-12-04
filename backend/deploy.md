# Deploy backend app to the Linux machine

- [Install PostgreSQL database engine](#install-postgresql-database-engine)
- [Install NGINX](#install-nginx)
- [Backend initial setup as service](#backend-initial-setup-as-service)
- [Setup NGINX](#setup-nginx)

## Install PostgreSQL database engine

[Article](https://pimylifeup.com/raspberry-pi-postgresql/)

- Install

```bash
sudo apt update
sudo apt full-upgrade

sudo apt install postgresql
```

- Change postgres user

```bash
sudo su postgres

createuser pi -P --interactive
# enter password when prompted
exit
```

## Install NGINX

[Article](https://pimylifeup.com/raspberry-pi-nginx/)

```bash
sudo apt install nginx
sudo systemctl start nginx
```

## Backend initial setup as service

- Change values in `appsettings.json`

- Publish backend as self-contained app. And copy it to the Linux machine:

```bash
# Linux machine
mkdir projects
cd projects
mkdir weather

# Windows machine
cd cd ./WeatherStationBackend

dotnet publish -c release -r linux-arm64 --self-contained

scp -r ./bin/release/net8.0/linux-arm64 pi@192.168.0.65:/home/pi/projects/weather
```

- Using ssh, add run permissions and run app (just for test, in next step, we should create a service to run it):

```bash
cd projects/weather
chmod +x ./WeatherStationBackend
./WeatherStationBackend
```

- Copy a service file

```bash
scp -r ./weather.service pi@192.168.0.65:/home/pi/projects/weather
```

- Register a service as `systemctl`

ssh:

```bash
#Copy service file to the system dir:
sudo cp weather.service /etc/systemd/system/weather.service
# Restart daemon
sudo systemctl daemon-reload
# Start services
sudo systemctl start weather.service
# Enable auto start
sudo systemctl enable weather.service
```

- Migrate database on fitst start:

```bash
curl http://localhost:5030
curl http://localhost:5030/getsensordata?dateFrom=42
```

## Setup NGINX

- Setup nginx.config

```bash
sudo nano /etc/nginx/nginx.conf
```

Add this to http section:

```json
  map $http_connection $connection_upgrade {
    "~*Upgrade" $http_connection;
    default keep-alive;
  }

  server {
    listen        8830;
    server_name   weather-station.com *.weather-station.com;
    location / {
        proxy_pass         http://127.0.0.1:5030;
        proxy_http_version 1.1;
        proxy_set_header   Upgrade $http_upgrade;
        proxy_set_header   Connection $connection_upgrade;
        proxy_set_header   Host $host;
        proxy_cache_bypass $http_upgrade;
        proxy_set_header   X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header   X-Forwarded-Proto $scheme;

        # Configuration for ServerSentEvents
        proxy_buffering off;
    }
  }
```

```bash
sudo systemctl restart nginx.service
```

#### Access backend:

Internally:
http://127.0.0.1:5030

From outside:
http://192.168.0.65:8830/swagger/index.html
