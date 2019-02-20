var options = {packages: ['corechart','columnchart'], callback : drawChart};
google.load('visualization', '1', options);
// Load the Visualization API and the piechart package.
//google.load('visualization', '1.0', {'packages':['corechart','columnchart']});

// Set a callback to run when the Google Visualization API is loaded.
//google.setOnLoadCallback(drawChart);

// Callback that creates and populates a data table,
// instantiates the pie chart, passes in the data and
// draws it.
function drawChart() {

    // Create the data table.
    var data1 = google.visualization.arrayToDataTable([
        ['Library', 'ns', { role: 'style' }],
        ['CppAD', 926, 'blue'],
        ['ADOLC', 401, 'blue'],
        ['Adept', 304, 'blue'],
        ['FADBAD++', 222, 'blue'],
        ['SACADO', 68, 'blue'],
        ['CoDiPack', 65, 'blue'],
        ['FunG', 65, 'orange'],
        ['FunG (opt)', 60, 'orange'],
        ['Manual', 60, 'gray']
    ]);

    var data1_1 = google.visualization.arrayToDataTable([
        ['Library', 'ns', { role: 'style' }],
        ['CppAD', 558, 'blue'],
        ['ADOLC', 336, 'blue'],
        ['Adept', 45.8, 'blue'],
        ['FADBAD++', 13.9, 'blue'],
        ['SACADO', 4.0, 'blue'],
        ['CoDiPack', 1.7, 'blue'],
        ['FunG', 1.7, 'orange'],
        ['Manual', 1.7, 'gray']
    ]);

    var data1_2 = google.visualization.arrayToDataTable([
        ['Library', 'ns', { role: 'style' }],
        ['CppAD', 1781, 'blue'],
        ['ADOLC', 887, 'blue'],
        ['Adept', 48, 'blue'],
        ['FADBAD++', 8.1, 'blue'],
        ['SACADO', 40, 'blue'],
        ['CoDiPack', 2.0, 'blue'],
        ['FunG', 1.7, 'orange'],
        ['Manual', 1.7, 'gray']
    ]);
    
//    var data2 = google.visualization.arrayToDataTable([
//        ['Library', 'time/s', { role: 'style' }],
//        ['FADBAD++', 1.412, 'blue'],
//        ['FunG', 1.364, 'orange'],
//        ['Manual', 1.325, 'gray']
//    ]);

//    var data3 = google.visualization.arrayToDataTable([
//        ['Library', 'time/s', { role: 'style' }],
//        ['FADBAD++', 0.581, 'blue'],
//        ['SACADO', 0.586, 'blue'],
//        ['FunG', 0.499, 'orange'],
//        ['Manual', 0.475, 'gray']
//    ]);

    var data4 = google.visualization.arrayToDataTable([
        ['Function','Armadillo','DUNE','Eigen'],
        ['update', 2.587   , 2.512    , 2.159],
        ['d0'    , 0.000071, 0.000040 , 0.000064],
        ['d1'    , 0.593   , 0.601    , 0.275],
        ['d2'    , 3.332   , 3.404    , 2.234],
        ['d3'    , 19.227  , 15.852   , 12.026]
    ]);

    var data5 = google.visualization.arrayToDataTable([
        ['Function','Armadillo','DUNE','Eigen'],
        ['update', 2.982   , 2.780    , 2.836],
        ['d0'    , 0.000042, 0.000043 , 0.000049],
        ['d1'    , 1.247   , 0.988    , 1.035],
        ['d2'    , 7.080   , 6.044    , 5.880],
        ['d3'    , 40.810  , 35.792   , 33.987]
    ]);
    
    
    // Set chart options
    var options = {title:'Average computation time for the evaluation of value and gradient',
                    width:700,
                    height:300,
                    legend : { position:"none"},
                    vAxis: {
                        'title': 'nanoseconds',
                        'scaleType': 'mirrorLog'
                    }
                    };

        var options_muscle = {title:'Performance for a model of muscle tissue',
                    width:700,
                    height:300,
                    colors: ['blue','orange','gray'],
                    vAxis: {
                        title: 'time/s'
                    }
                    };

                    
    // Instantiate and draw our chart, passing in some options.
    var chart1 = new google.visualization.ColumnChart(document.getElementById('chart1_div'));
    chart1.draw(data1, options);
    var chart1_1 = new google.visualization.ColumnChart(document.getElementById('chart1_1_div'));
    chart1_1.draw(data1_1, options);
    var chart1_2 = new google.visualization.ColumnChart(document.getElementById('chart1_2_div'));
    chart1_2.draw(data1_2, options);
//    var chart2 = new google.visualization.ColumnChart(document.getElementById('chart2_div'));
//    chart2.draw(data2, options);
//    var chart3 = new google.visualization.ColumnChart(document.getElementById('chart3_div'));
//    chart3.draw(data3, options);
    var chart4 = new google.visualization.ColumnChart(document.getElementById('chart4_div'));
    chart4.draw(data4, options_muscle);
    var chart5 = new google.visualization.ColumnChart(document.getElementById('chart5_div'));
    chart5.draw(data5, options_muscle);
    
    MathJax.Hub.Queue(["Typeset",MathJax.Hub]);
    MathJax.Hub.Queue( function(){
        var contentHeight = $('.main-content').height();
        var headerOffset = 150;
        $('.background-image-container').height(contentHeight + headerOffset);
    });
}
