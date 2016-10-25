<!--Regions and country -->
<div class="row">
    <div class="col-sm-12 section-wrapper text-center" id="region_country_container">
        <h2 class="pane-title">Regions and Countries</h2>
        <div class="country-tab draggable-container">
            <div class="draggable draggable-center ui-draggable ui-draggable-handle">
                <!-- Nav tabs -->
                <ul class="row nav nav-tabs" role="tablist">
                    <li class="col-xs-6 col-sm-4 col-md-2" role="presentation" class="active"> <a href="#africa_sec" aria-controls="africa_sec" role="tab" data-toggle="tab"> <span class="regions-icons"><i class="sprite icon-africa"></i></span> AFRICA</a></li>
                    <li class="col-xs-6 col-sm-4 col-md-2" role="presentation" > <a href="#east_asia_sec" aria-controls="east_asia_sec" role="tab" data-toggle="tab"> <span class="regions-icons"><i class="sprite icon-eastasia"></i></span> EAST ASIA AND PACIFIC</a></li>
                    <li class="col-xs-6 col-sm-4 col-md-2" role="presentation"> <a href="#europe_sec" aria-controls="europe_sec" role="tab" data-toggle="tab"> <span class="regions-icons"><i class="sprite icon-europe"></i></span> EUROPE AND CENTRAL ASIA</a></li>
                    <li class="col-xs-6 col-sm-4 col-md-2" role="presentation"><a href="#latin_america_sec" aria-controls="latin_america_sec" role="tab" data-toggle="tab"> <span class="regions-icons"><i class="sprite icon-latinamerica"></i></span> LATIN AMERICA AND CARIBBEAN</a></li>
                    <li class="col-xs-6 col-sm-4 col-md-2" role="presentation"><a href="#middle_east_sec" aria-controls="middle_east_sec" role="tab" data-toggle="tab"> <span class="regions-icons"><i class="sprite icon-middlewast"></i></span> MIDDLE EAST AND NORTH AFRICA</a></li>
                    <li class="col-xs-6 col-sm-4 col-md-2" role="presentation"><a href="#southasia_sec" aria-controls="southasia_sec" role="tab" data-toggle="tab"> <span class="regions-icons"><i class="sprite icon-southasia"></i></span> SOUTH ASIA</a></li>
                </ul>
            </div>
            <!-- Tab panes -->
            <div class="tab-content text-left">
                <div role="tabpanel" class="tab-pane active" id="africa_sec">
                    <ul class="topics-list-data">
                        <?php foreach ($countries as $key => $value) : ?>
                            <?php if (in_array($countries[$key]["name"], $africa)) : ?>
                                <?php print "<li class=\"col-sm-3 col-xs-6\"><a href=\"" . url('/search/field_sop_country/' . strtolower($countries[$key]["name"]) . '--' . $countries[$key]["uuid"]) . "\">" . $countries[$key]["name"] . "</a></li>"; ?>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </ul>
                </div>
                <div role="tabpanel" class="tab-pane" id="east_asia_sec">
                    <ul class="topics-list-data">
                        <?php foreach ($countries as $key => $value) : ?>
                            <?php if (in_array($countries[$key]["name"], $east_asia_n_pacific)) : ?>
                                <?php print "<li class=\"col-sm-3 col-xs-6\"><a href=\"" . url('/search/field_sop_country/' . strtolower($countries[$key]["name"]) . '--' . $countries[$key]["uuid"]) . "\">" . $countries[$key]["name"] . "</a></li>"; ?>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </ul>
                </div>
                <div role="tabpanel" class="tab-pane" id="europe_sec">
                    <ul class="topics-list-data">
                        <?php foreach ($countries as $key => $value) : ?>
                            <?php if (in_array($countries[$key]["name"], $europe_n_central_asia)) : ?>
                                <?php print "<li class=\"col-sm-3 col-xs-6\"><a href=\"" . url('/search/field_sop_country/' . strtolower($countries[$key]["name"]) . '--' . $countries[$key]["uuid"]) . "\">" . $countries[$key]["name"] . "</a></li>"; ?>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </ul>
                </div>
                <div role="tabpanel" class="tab-pane" id="latin_america_sec">
                    <ul class="topics-list-data">
                        <?php foreach ($countries as $key => $value) : ?>
                            <?php if (in_array($countries[$key]["name"], $latin_america_n_caribbean)) : ?>
                                <?php print "<li class=\"col-sm-3 col-xs-6\"><a href=\"" . url('/search/field_sop_country/' . strtolower($countries[$key]["name"]) . '--' . $countries[$key]["uuid"]) . "\">" . $countries[$key]["name"] . "</a></li>"; ?>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </ul>
                </div>
                <div role="tabpanel" class="tab-pane" id="middle_east_sec">
                    <ul class="topics-list-data">
                        <?php foreach ($countries as $key => $value) : ?>
                            <?php if (in_array($countries[$key]["name"], $middle_east_n_north_africa)) : ?>
                                <?php print "<li class=\"col-sm-3 col-xs-6\"><a href=\"" . url('/search/field_sop_country/' . strtolower($countries[$key]["name"]) . '--' . $countries[$key]["uuid"]) . "\">" . $countries[$key]["name"] . "</a></li>"; ?>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </ul>
                </div>
                <div role="tabpanel" class="tab-pane" id="southasia_sec">
                    <ul class="topics-list-data">
                        <?php foreach ($countries as $key => $value) : ?>
                            <?php if (in_array($countries[$key]["name"], $south_asia)) : ?>
                                <?php print "<li class=\"col-sm-3 col-xs-6\"><a href=\"" . url('/search/field_sop_country/' . strtolower($countries[$key]["name"]) . '--' . $countries[$key]["uuid"]) . "\">" . $countries[$key]["name"] . "</a></li>"; ?>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </ul>
                </div>
            </div>
        </div>
    </div>
</div>
<!--Regions and country end-->
