# Enable HTTPS everywhere

When you want to enable https everywhere on a dkan site. 

## Step-by-step guide

You'll need to take several steps to enable https everywhere for a site instance on Acquia.

1. To config/config.yml add:
  ```
  default:
    https_everywhere: TRUE
  ```

2. Rebuild the site configuration with ahoy:
  ```
  ahoy build config 
  ```
3. Commit and push changes to Acquia
4. On the Acquia dashboard, enable a varnish proxy SSL: https://docs.acquia.com/cloud/performance/varnish#ssl (per development environment)
