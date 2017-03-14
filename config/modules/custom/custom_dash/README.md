### Custom data handlers
To create custom data handlers you need to create a dataHandlers.js file within the js folder and populate the `Drupal.settings.dkanDash.dataHandlers` global with the handlers you want to create.

```javascript
Drupal.settings.dkanDash.dataHandlers = {
    handler1: function() {
        //stuff
    },
    handler2: function() {
        //stuff
    },    
}
```

### Custom CSS
To create custom css create file called `custom.css` inside the css folder.