<?

global $db;

if($_POST['action'] == 'alterarSprint' && $_POST['idpostit']){
    
    $sql = '';
    if($_POST['arrPostitId']){
        
        if(!$_POST['idsprint']) $_POST['idsprint'] = 'null';
        
        $editar = false;
        foreach($_POST['arrPostitId'] as $key => $idPostit){
            
            if($idPostit == $_POST['idpostit']) $editar = true;
            
            $ordem = $key + 1;
            
            if($editar){
                $sql .= "UPDATE scrum.entregavel SET sptid = {$_POST['idsprint']} , entordsprint  = {$ordem} WHERE entid = {$idPostit} RETURNING entid; ";
            }
        }
    }
    
    $idEntregavel = $db->pegaUm($sql);
    $db->commit();
    
    echo $idEntregavel;
    exit;
}

// Pegando o id do programa nos dados post se não tiver pega no cookie.
if (isset($_POST['prgid'])) {
    $prgid = $_POST['prgid'];
    setcookie("prgid", $prgid, time() + 60 * 60 * 24 * 30, "/");
} else if (isset($_COOKIE["prgid"])) {
    $prgid = $_COOKIE["prgid"];
} else {
    $prgid = '';
}


if($_POST['sptid']){
    $sqlSprintAtual = "SELECT sptid ,
                       TO_CHAR( sptinicio , 'dd/mm/yyyy' ) as sptinicio, 
                       TO_CHAR( sptfim , 'dd/mm/yyyy' ) as sptfim FROM scrum.sprint 
                       WHERE sptid = {$_POST['sptid']}";
} else {
    $sqlSprintAtual = 'SELECT sptid , 
                       TO_CHAR( sptinicio , \'dd/mm/yyyy\' ) as sptinicio, 
                       TO_CHAR( sptfim , \'dd/mm/yyyy\' ) as sptfim FROM scrum.sprint 
                       WHERE now() 
                       between sptinicio and sptfim';
}
$sprintAtual = $db->pegaLinha($sqlSprintAtual);

$entregaveisSprintAtual = array();
$entregaveisBackLog = array();
if($prgid){
    
    if($sprintAtual){
        
    
        $sqlPrograma = "SELECT * FROM scrum.programa WHERE prgid = {$prgid}";
        $programa = $db->pegaLinha($sqlPrograma);

        $idSprintAnterior = $sprintAtual['sptid'] - 1;
        $idSprintAtual = $sprintAtual['sptid'];
        $idProximaSprint = $sprintAtual['sptid'] + 1;

        $sqlSprintAnterior = 'SELECT sptid , TO_CHAR( sptinicio , \'dd/mm/yyyy\' ) as sptinicio , TO_CHAR( sptfim , \'dd/mm/yyyy\' ) as sptfim FROM scrum.sprint 
                            WHERE sptid = ' . $idSprintAnterior;
        $sprintAnterior = $db->pegaLinha($sqlSprintAnterior);

        $sqlProximaSprint = 'SELECT sptid , TO_CHAR( sptinicio , \'dd/mm/yyyy\' ) as sptinicio , TO_CHAR( sptfim , \'dd/mm/yyyy\' ) as sptfim FROM scrum.sprint 
                            WHERE sptid = ' . $idProximaSprint;
        $proximaSprint = $db->pegaLinha($sqlProximaSprint);

        // ENTREGAVEIS SPRINT ATUAL
        $sqlEntregaveisSptrinAtual = "SELECT 
                        prg.prghrsprint,
                        ent.enthrsexec,
                        ent.entordsprint,
                        ent.entdsc,
                        ent.entid,
                        spg.subprgdsc,
                        spg.subprgcolor,
                        est.esttitulo,
                        ent.sptid,
                        COALESCE(usu.usunome, '') AS usucpfresp_dsc
                FROM scrum.entregavel ent
                    INNER JOIN scrum.estoria est USING(estid)
                    INNER JOIN scrum.subprg spg USING(subprgid)
                    INNER JOIN scrum.programa prg USING(prgid)
                    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                    LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
                WHERE prgid = {$prgid}
                AND spt.sptid = {$sprintAtual['sptid']}
                ORDER BY ent.entordsprint ASC, ent.entid DESC";
        $entregaveisSprintAtual = $db->carregar($sqlEntregaveisSptrinAtual);

        $sprintAtualHorasGastas = 0;
        if(is_array($entregaveisSprintAtual)){
            foreach($entregaveisSprintAtual as $entregavel){
                $sprintAtualHorasGastas += $entregavel['enthrsexec'];
            }
        }

        //PROXIMA SPRINT 
        $sqlSptrinAnterior = "SELECT 
                        prg.prghrsprint,
                        ent.enthrsexec,
                        ent.entordsprint,
                        ent.entdsc,
                        ent.entid,
                        spg.subprgdsc,
                        spg.subprgcolor,
                        est.esttitulo,
                        ent.sptid,
                        COALESCE(usu.usunome, '') AS usucpfresp_dsc
                FROM scrum.entregavel ent
                    INNER JOIN scrum.estoria est USING(estid)
                    INNER JOIN scrum.subprg spg USING(subprgid)
                    INNER JOIN scrum.programa prg USING(prgid)
                    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                    LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
                WHERE prgid = {$prgid}
                AND spt.sptid = {$idSprintAnterior}
                ORDER BY ent.entordsprint ASC, ent.entid DESC";
        $entregaveisSprintAnterior = $db->carregar($sqlSptrinAnterior);

        $sprintAnteriorHorasGastas = 0;
        if(is_array($entregaveisSprintAnterior)){
            foreach($entregaveisSprintAnterior as $sprint){
                $sprintAnteriorHorasGastas += $sprint['enthrsexec'];
            } 
        }

        //PROXIMA SPRINT 
        $sqlProximaSptrin = "SELECT 
                        prg.prghrsprint,
                        ent.enthrsexec,
                        ent.entordsprint,
                        --ent.entdsc,
                        ent.entid,
                        spg.subprgdsc,
                        spg.subprgcolor,
                        est.esttitulo,
                        ent.sptid,
                        COALESCE(usu.usunome, '') AS usucpfresp_dsc
                FROM scrum.entregavel ent
                    INNER JOIN scrum.estoria est USING(estid)
                    INNER JOIN scrum.subprg spg USING(subprgid)
                    INNER JOIN scrum.programa prg USING(prgid)
                    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                    LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
                WHERE prgid = {$prgid}
                AND spt.sptid = {$idProximaSprint}
                ORDER BY ent.entordsprint ASC, ent.entid DESC";
        $entregaveisProximaSprint = $db->carregar($sqlProximaSptrin);

        $proximaSprintHorasGastas = 0;
        if(is_array($entregaveisProximaSprint)){
            foreach($entregaveisProximaSprint as $sprint){
                $proximaSprintHorasGastas += $sprint['enthrsexec'];
            } 
        }
    }
    
    // BACKLOG
    $sqlEntregaveisBackLog = "SELECT 
                                    prg.prghrsprint,
                                    ent.enthrsexec,
                                    ent.entordsprint,
                                    ent.entdsc,
                                    ent.entid,
                                    spg.subprgdsc,
                                    spg.subprgcolor,
                                    est.esttitulo,
                                    ent.sptid,
                                    COALESCE(usu.usunome, '') AS usucpfresp_dsc
                            FROM scrum.entregavel ent
                                INNER JOIN scrum.estoria est USING(estid)
                                INNER JOIN scrum.subprg spg USING(subprgid)
                                INNER JOIN scrum.programa prg USING(prgid)
                                LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                                --LEFT JOIN scrum.sprint ON spt (spt.sptid = ent.sptid)
                            WHERE prgid = {$prgid}
                            AND sptid IS NULL
                                ORDER BY ent.entordsprint ASC, ent.entid DESC";
    $entregaveisBackLog = $db->carregar($sqlEntregaveisBackLog);
    
//    ver($sprintAtual);
} 

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'www/scrum/funcoesPrograma.php';
include APPRAIZ . 'www/scrum/funcoesEntregavel.php';
include APPRAIZ . 'www/scrum/funcoesBoard.php';

?>

<?php // montaComboPrograma($_POST['prgid']); ?>
<style>
    /**{padding:0;margin:0;}*/
    .keyboard_shortcut.highlight{color:#00f;}#head #sub-nav #sub-nav-pro a#close-dash .keyboard_shortcut.highlight,#head #sub-nav #sub-nav-pro a#open-dash.pressed .keyboard_shortcut.highlight,.dash_btn_grp .keyboard_shortcut.highlight{color:#fff;text-decoration:underline;}.clearfix:after{content:".";display:block;height:0;clear:both;visibility:hidden;}.clearfix{display:inline-block;} Hide from IE Mac \ .clearfix{display:block;} End hide from IE Mac  body.default{background:url(/images/bg.gif) repeat-x #fff;color:#333;font-family:Arial,Sans;margin:10px;padding:0;font-size:16px;}.default img{border:0;}.default h1{display:inline;font-family:Rockwell,Arial;font-size:22px;letter-spacing:-2px;line-height:1em;}
    #scrumy-board{clear:both;position:relative;top:2px;width:100%;padding:0;margin:0;}
    /*.alert{font-size:12px;font-weight:bold;left:-10px;opacity:.9;filter:alpha(opacity=90);position:relative;text-align:center;top:-10px;width:100%;padding:0 10px;}.alert span{display:block;padding:10px;}.alert.autoswitch{background-color:#FF9;color:black;border-bottom:1px solid #EE9;}.alert.site_wide_notice{background-color:#def;border-bottom:1px solid #cdd;color:#222;}.alert.unsupported-browser{background-color:#fdd;border-bottom:1px solid #dcc;color:#222;font-weight:normal;}.alert.empty-board{background-color:#dfd;border-bottom:1px solid #cdc;color:#222;}.noselection{cursor:default;}*/
    #scrumy-board tr td{padding:1px 0 1px 1px;}
    #scrumy-board tr td.drop-zone{padding:0;}
    #scrumy-board tr td.todo.drop-zone{border:1px solid #e5e5e5;}
    #scrumy-board tr td.inprogress.drop-zone{border:1px solid #e2e2e2;}
    #scrumy-board tr td.verify.drop-zone{border:1px solid #e5e5e5;}
    #scrumy-board tr td.done.drop-zone{border:1px solid #e7e7e7;}
    
    #scrumy-board .active-story .stories{background-color:#f7f7f7;}
    #scrumy-board .active-story .todo{background-color:#f5f5f5;}
    #scrumy-board .active-story .inprogress{background-color:#f2f2f2;}
    #scrumy-board .active-story .verify{background-color:#f5f5f5;}
    #scrumy-board .active-story .done{background-color:#f7f7f7;}
    #scrumy-board .active-story .stories .story{border-color:#dfdfdf;background-color:#f9f9f9;}
    #head{top:0;left:0;right:0;height:29px;}
    
    #head h1.title{float:left;margin:2px 0 8px 6px;padding:0 6px 0 0;}#head #status-messages{color:#666;font-size:14px;padding-top:4px;position:absolute;text-align:center;width:100%;left:0;}#head #nav{float:left;margin-top:8px;}#head #nav,#head #nav a{color:#999;font-size:12px;}#head #nav a{padding:4px;}#head #nav a:hover,#head #sub-nav a:hover{color:#333;}#head #nav a#comet-status.online{color:#596;}#head #nav a#comet-status.offline{color:#956;}.readonly_notification{background-color:#ffc;font-size:.9em;margin-left:5px;padding:2px 5px;text-transform:uppercase;}#head #sub-nav.upgrade-link a{font-size:12px;}#head #sub-nav.upgrade-link{padding-top:8px;}#head #sub-nav{text-align:right;padding-right:8px;}#head #sub-nav,#head #sub-nav a{color:#999;font-size:11px;}#head #sub-nav a{padding:5px 10px;margin-top:4px;}#head #sub-nav #sub-nav-pro{white-space:nowrap;}#head #sub-nav #sub-nav-pro a{background-color:#f6f6f6;padding-bottom:12px;text-decoration:none;border-left:1px solid #eee;border-right:1px solid #ddd;border-top:1px solid #eee;display:inline-block;}#head #sub-nav #sub-nav-pro a:hover{background-color:#222;border:solid 1px #555;color:#fff;text-decoration:none;}#head #sub-nav #sub-nav-pro a#close-dash,#head #sub-nav #sub-nav-pro a#open-dash.pressed{background:url(/images/dashboard/dash-barbg.png) repeat-x 0 0;padding-bottom:0;padding-top:6px;color:#fff;border:0;margin-top:4px;outline:none;height:22px;*height:26px;}#current-burndown{position:absolute;right:10px;text-align:center;top:43px;}#current-burndown #cb-image{-moz-border-radius:2px;background-color:#F0F0F0;border:3px solid #1C1C1C;display:block;position:relative;z-index:1002;width:400px;height:285px;padding-top:15px;}#open-burndown.pressed span{color:#fff;}#open-burndown.pressed span.highlight{text-decoration:underline;}#cb-image-mini{width:300px;height:200px;padding-top:10px;text-align:center;}#current-burndown #cb-image #close-burndown{position:absolute;top:5px;right:10px;font-size:24px;text-decoration:none;}#cb-overlay{background-color:transparent;}#head #sub-nav #sub-nav-pro a#open-burndown.pressed{background-color:#1c1c1c;color:#fff;}#head #sub-nav a.disabled-burndown{color:#ccc;position:relative;z-index:-100;}#cb-image h1,#cb-image span,#cb-image-mini h5,#cb-image-mini span{color:#fff;}#cb-image h1,#cb-image-mini h5{display:inline;font-family:Rockwell,Arial;letter-spacing:-2px;line-height:1em;font-weight:normal;}#cb-image h1{font-size:30px;}#cb-image-mini h5{font-size:21px;}#cb-image span.chart-daterange,#cb-image-mini span.chart-daterange{display:inline-block;letter-spacing:-1px;}#cb-image span.chart-daterange{font-size:18px;padding-bottom:5px;}#cb-image-mini span.chart-daterange{font-size:15px;padding-bottom:6px;}#cb-image span.chart-estimated,#cb-image span.chart-actual,#cb-image span{font-size:13px;}#cb-image-mini span.chart-estimated,#cb-image-mini span.chart-actual,#cb-image-mini span{font-size:11px;}#cb-image span.chart-estimated,#cb-image span.chart-actual,#cb-image-mini span.chart-estimated,#cb-image-mini span.chart-actual{text-transform:uppercase;}#cb-image span.chart-estimated,#cb-image-mini span.chart-estimated{color:#007eff;}#cb-image span.chart-actual,#cb-image-mini span.chart-actual{color:#00c25b;}#head,#footer,#scrumy-board{min-width:650px;}#footer,#footer a{color:#ccc;padding:5px;font-size:11px;}#footer{padding-top:27px;}#footer a:hover{color:#333;}#footer img{vertical-align:-3px;margin-right:3px;opacity:.55;filter:alpha(opacity=55);}#footer #legal{float:left;}#footer #links{float:right;}#footer #local_settings{background-color:white;border:1px solid #ccc;color:#999;margin-top:-55px;padding:9px 11px;position:absolute;*margin-top:-35px;*margin-left:-145px;}#footer #local_settings_link{outline:none;}#footer #local_settings_link.selected{border:solid 1px #eee;text-decoration:none;margin:-4px;padding:4px 8px;}#footer .switch{display:inline-block;font-family:Arial;height:12px;padding:2px 8px 4px;border:1px solid;-moz-border-radius:5px;-webkit-border-radius:5px;border-radius:5px;cursor:pointer;}.mac #footer .switch{padding:3px 8px;}#footer .switch.left{-moz-border-radius-bottomright:0;-moz-border-radius-topright:0;-webkit-border-top-right-radius:0;-webkit-border-bottom-right-radius:0;border-top-right-radius:0;border-bottom-right-radius:0;}#footer .switch.right{-moz-border-radius-bottomleft:0;-moz-border-radius-topleft:0;-webkit-border-top-left-radius:0;-webkit-border-bottom-left-radius:0;border-top-left-radius:0;border-bottom-left-radius:0;margin-left:-4px;}#footer .switch.selected{background-color:#999;border-color:#666;color:#fff;}#footer #se-local{margin-top:10px;padding-bottom:10px;}#footer #se-local p{color:#aab;display:inline;margin-right:39px;}#footer .swimlane_grp{padding-right:10px;}#footer .swimlane_grp #swimlane_button,#footer .swimlane_grp #grid_button{background-image:url(/images/swimlane-icons.gif);background-repeat:no-repeat;padding-left:23px;}#footer .swimlane_grp #swimlane_button{background-position:6px 4px;}#footer .swimlane_grp #swimlane_button.selected{background-position:-194px 4px;}#footer .swimlane_grp #grid_button{background-position:-94px 4px;border-left:1px solid #666;}#footer .swimlane_grp #grid_button.selected{background-position:-294px 4px;}#task_editor .grey_text{color:#999;}#task_editor textarea{background-color:transparent;border:1px inset;color:inherit;float:left;font-family:Arial;font-size:20px;height:127px;line-height:1;margin:6px;padding:0 5px;text-align:center;width:176px;resize:none;}.plus textarea{font-family:Arial;font-size:11px;overflow-x:hidden;text-align:center;width:60px;}.invisible-task{visibility:hidden;}
    table#scrumy-board th span{margin-left:7px;margin-right:7px;}
    table#scrumy-board{border-style:solid;border-color:#f0f0f0;border-width:1px 1px 1px 1px;background-color:#f0f0f0;empty-cells:show;}
    
    table#scrumy-board th,table#scrumy-board td{border-style:solid;border-color:#f0f0f0;border-width:0 0px 0 0;margin:0;}
    
    table#scrumy-board th.separator,table#scrumy-board td.separator{width:1px;padding:0;margin:0;}table#scrumy-board th{font-weight:normal;font-style:italic;height:36px;color:#666;font-size:12px;cursor:default;}

    table#scrumy-board th#story-header{background-color:#fcfcfc;width:150px;text-align:left;text-transform:uppercase;font-size:11px;}
    table#scrumy-board th#story-header span{padding-left:9px;}
    table#scrumy-board .stories{
        background:#fcfcfc;
        text-align:center;
        vertical-align:middle;
        /*font-weight: bold;*/
        font-size: 14px;
        /*font-family:Arial;*/
    }

    table#scrumy-board th.header{background-color:#fcfcfc;width:150px;text-align:center;text-transform:uppercase;font-size:11px;}
    
    table#scrumy-board th#plus-header,table#scrumy-board .plus{background:#f7f7f7;vertical-align:middle;text-align:center;width:18px;}
    table#scrumy-board .plus-button{opacity:.5;filter:alpha(opacity=50);}table#scrumy-board .plus-button:hover{opacity:1.0;filter:alpha(opacity=100);}
    #scrumy-board #todo-header{background-color:#fafafa;}#scrumy-board .todo,#scrumy-board .ph-todo{background:#fafafa;}#scrumy-board #inprogress-header{background-color:#f7f7f7;}
    #scrumy-board .inprogress,#scrumy-board .ph-inprogress{background:#f7f7f7;}#scrumy-board #verify-header{background-color:#fafafa;}#scrumy-board .verify,#scrumy-board .ph-verify{background:#fafafa;}#scrumy-board #done-header{background-color:#fcfcfc;}#scrumy-board .done,#scrumy-board .ph-done{background:#fcfcfc;}#scrumy-board td.activate{background-color:#aaa;}#scrumy-board #todo-header,#scrumy-board #inprogress-header,#scrumy-board #verify-header,#scrumy-board #done-header{white-space:nowrap;}#scrumy-board td{width:auto;vertical-align:top;}#scrumy-board td ul{list-style:none;}
    .task,.invisible-task{
        float:left;
        margin-top:9px;
        margin-bottom:6px;
        margin-left:5px;
        opacity:1.0;
        position:relative;
        width:200px;
        height:160px;
        cursor:default;
        line-height:12px;
        /*transparent*/
        border: 1px solid  #f0f0f0;
        -moz-border-radius: 2px;
        -webkit-border-radius: 2px;
        border-radius: 4px;
        
        box-shadow: 2px 2px 12px -3px #222222;
    }
    .task .handle{
        position:absolute;
        width:190px;
        height:22px;
        left:4px;
        /*cursor:move;*/
        z-index:99; 
        padding-top: 6px;
        text-shadow: 0px 0px 1px #FFFFFF;
        color: #FFFFFF;
        font-size: 12px;
        text-align: center;
    }
    .readonly .task .handle{cursor:default;}
    .task .title,.invisible-task .title {
        position:absolute;
        top: 25px;
        left: 1px;
        width: 196px;
        height: 132px;
        /*text-align: center;*/
        font-size: 11px;
        overflow: hidden;
    }
/*    .title b{
        text-align: center;
    }*/
    
    
    
    .task .title .title_inner,
    .invisible-task .title .title_inner,#tooltip pre{
        font-family:Arial;
        /*        white-space:pre-wrap;
                word-wrap:break-word;*/
        /*display:inline;*/ 
        font-size: 11px;
        
        
        position: relative;
        font-size: 12px;
        /*background-color: #f7f7f7;*/
        background-color: #FFFDE4;
        border: 1px solid #f0f0f0;
        -moz-border-radius: 2px;
        -webkit-border-radius: 2px;
        border-radius: 2px;
        height: 132px;
/*        bottom: 1px;*/
        
    }
    .task .title textarea{display:none;}
    .task .assignment{
        font-size: 13px;
        position: absolute;
        bottom: 1px;
        right: 1px;
        padding-left: 0px;
        line-height: 1;
        display: block;
        overflow: hidden;
        width: 30px;
        height: 16px;
        text-align: center;
        border: 1px solid #f0f0f0;
        -moz-border-radius: 2px;
        -webkit-border-radius: 2px;
        border-radius: 3px;
        background-color: #FFF;
        /*display: none;*/
    }
    .task:hover .iconEdit{
        display:block;
    }
    
    
    .task .iconEdit{
        font-size: 12px;
        position: absolute;
        bottom: 18px;
        right: 1px;
        padding-left: 0px;
        line-height: 1;
        display: block;
        overflow: hidden;
        width: 20px;
        height: 16px;
        text-align: center;
        display: none;
        cursor: pointer;
        
        border: 1px solid #f0f0f0;
        -moz-border-radius: 2px;
        -webkit-border-radius: 2px;
        border-radius: 3px;
        background-color: #FFF;
    }
    
    .task:hover .handle{
        text-shadow: 0px 0px 3px #FFFFFF;
    }
    
    .task .assignment.unassigned{color:#ccc;}
    .task .overflow_indicator{display:none;}
    .task.overflowing .overflow_indicator{display:block;position:absolute;right:6px;top:64px;}
    .task.overflowing .assignment{width:56px;}.task.pending{opacity:.5;filter:alpha(opacity=50);}
    .story-task-row.pending .story{opacity:.5;}
    .white,.yellow,.mint,.pink,.green,.violet,.grey,.lime,.blue,.goldenrod,.seaweed,.chocolate,.halloween,.tropics,.sponge,.aftervibe-red,.def-blue,.teal,.dark,.tabloid{background-image:url(/imagens/all-postits.png);background-repeat:no-repeat;}.white{background-position:0 0;}.yellow{background-position:-88px 0;}.mint{background-position:-176px 0;}.pink{background-position:-264px 0;}.green{background-position:-352px 0;}.violet{background-position:-440px 0;}.grey{background-position:-528px 0;}.lime{background-position:-616px 0;}.blue{background-position:-704px 0;}.seaweed{background-position:-880px 0;}.goldenrod{background-position:-792px 0;}.chocolate{background-position:-968px 0;}.aftervibe-red{background-position:-1056px 0;}.def-blue{background-position:-1144px 0;}.teal{background-position:-1232px 0;}.dark{background-position:-1320px 0;}.halloween{background-position:-1408px 0;}.tropics{background-position:-1496px 0;}.sponge{background-position:-1584px 0;}.tabloid{background-position:-1672px 0;}#scrumer-color-chooser .white{background-position:-10px -7px;}#scrumer-color-chooser .yellow{background-position:-93px -7px;}#scrumer-color-chooser .mint{background-position:-181px -7px;}#scrumer-color-chooser .pink{background-position:-269px -7px;}#scrumer-color-chooser .green{background-position:-357px -7px;}#scrumer-color-chooser .violet{background-position:-444px -7px;}#scrumer-color-chooser .grey{background-position:-533px -7px;}#scrumer-color-chooser .lime{background-position:-621px -7px;}#scrumer-color-chooser .blue{background-position:-709px -7px;}#scrumer-color-chooser .seaweed{background-position:-885px -7px;}#scrumer-color-chooser .goldenrod{background-position:-797px -7px;}#scrumer-color-chooser .chocolate{background-position:-973px -7px;}#scrumer-color-chooser .aftervibe-red{background-position:-1061px -7px;}#scrumer-color-chooser .def-blue{background-position:-1149px -7px;}#scrumer-color-chooser .teal{background-position:-1237px -7px;}#scrumer-color-chooser .dark{background-position:-1325px -7px;}#scrumer-color-chooser .halloween{background-position:-1413px -7px;}#scrumer-color-chooser .tropics{background-position:-1501px -7px;}#scrumer-color-chooser .sponge{background-position:-1589px -7px;}#scrumer-color-chooser .tabloid{background-position:-1677px -7px;}#task_editor .white{background-color:white;}#task_editor .yellow{background-color:#fbfbd5;}#task_editor .mint{background-color:#e7f5fb;}#task_editor .pink{background-color:#f9e6f2;}#task_editor .green{background-color:#ddfbd6;}#task_editor .violet{background-color:#ece2fb;}#task_editor .grey{background-color:#f2f2f2;}#task_editor .lime{background-color:#f6fbb8;}#task_editor .blue{background-color:#cee6f4;}#task_editor .goldenrod{background-color:#fbe680;}#task_editor .seaweed{background-color:#a8d58c;}#task_editor .chocolate{background-color:#c9b6b2;}#task_editor .aftervibe-red{background-color:#b00032;}#task_editor .def-blue{background-color:#313193;}#task_editor .teal{background-color:#00a6b5;}#task_editor .dark{background-color:#535452;}#task_editor .halloween{background-color:#fb6f2a;}#task_editor .tropics{background-color:#3be4f8;}#task_editor .sponge{background-color:#f7ee64;}#task_editor .tabloid{background-color:#fb3c89;}.white,.yellow,.mint,.pink,.green,.violet,.grey,.lime,.blue,.goldenrod,.seaweed,.chocolate,.halloween,.tropics,.sponge{color:#000;}.aftervibe-red,.def-blue,.teal,.dark,.tabloid{color:#fff;}#task_editor .white .overflowing,#task_editor .yellow .overflowing,#task_editor .mint .overflowing,#task_editor .pink .overflowing,#task_editor .green .overflowing,#task_editor .violet .overflowing,#task_editor .grey .overflowing,#task_editor .lime .overflowing,#task_editor .blue .overflowing,#task_editor .goldenrod .overflowing,#task_editor .seaweed .overflowing,#task_editor .chocolate .overflowing,#task_editor .halloween .overflowing,#task_editor .tropics .overflowing,#task_editor .sponge .overflowing{background-color:#eee;background-image:url(/images/overflow_indicator.gif);background-position:178px 119px;background-repeat:no-repeat;}#task_editor .aftervibe-red .overflowing,#task_editor .def-blue .overflowing,#task_editor .teal .overflowing,#task_editor .dark .overflowing,#task_editor .tabloid .overflowing{background-color:#666;}.task a.edit-task{display:none;position:absolute;right:5px!important;top:52px!important;z-index:10;}
    .task:hover a.edit-task{display:block;}
    .ios .move_task-left,.ios .move_task-right,.ios .task .edit-task,.ios .story .edit-story{background-color:#eee;border-color:#555;border-width:1px;color:black;font-size:13px;font-weight:normal;-webkit-border-radius:20px;text-decoration:none;width:auto;}
    .ios .task .edit-task,.ios .story .edit-story{background:-webkit-gradient(linear,0 0,left bottom,color-stop(0,#fff),color-stop(100%,#ccc)) no-repeat;}
    .ios .task .edit-task{top:60px!important;right:10px!important;padding:4px 10px 6px;}
    .ios .story .edit-story{top:50px!important;right:45px!important;padding:3px 10px 4px;bottom:auto;}.ios .task a.delete-task{position:relative;left:-2px!important;top:-4px!important;}.ios .story a.delete-story{left:-7px!important;top:-7px!important;}.ios .task a.delete-task,.ios .story a.delete-story{height:16px;width:16px;margin-bottom:2px;overflow:hidden;text-indent:-99999px;-webkit-background-size:16px;background:transparent url(/images/ipad-delete.png) no-repeat 0 0;border:none;}.move_task-left,.move_task-right{display:none;}.ios .move_task-left,.ios .move_task-right{border-style:solid;display:none;position:absolute;padding:5px;top:20px;z-index:100;height:25px;width:25px;-webkit-border-radius:25px;margin-bottom:2px;overflow:hidden;text-indent:-99999px;-webkit-background-size:165%;}.ios .move_task-left{left:-10px;background:transparent url(/images/ipad-move-left.png) no-repeat -11px -11px;}.ios .move_task-right{right:-11px;background:transparent url(/images/ipad-move-right.png) no-repeat -11px -11px;}.ios .task:hover .move_task-left,.ios .task:hover .move_task-right{display:block;}.ios .todo .task:hover .move_task-left{display:none;}.ios .done .task:hover .move_task-right{display:none;}.ios .task:hover a.delete-task{display:inline-block;}.ios #task_editor_buttons input{padding-bottom:0;padding-top:1px;}.task a.delete-task{display:none;position:absolute;right:5px!important;top:3px!important;z-index:100;overflow:hidden;outline:none;}.task:hover a.delete-task{display:block;}a.edit-task,a.edit-story,#dash a.select-all{background-color:#fff;border:1px solid #999;color:#000;text-decoration:none;padding:2px 5px 3px;font-size:10px;font-weight:bold;outline:none;font-style:normal;}#dash a.select-all{margin-right:7px;}
    /*a.edit-task:active,a.edit-story:active,#dash a.select-all:active{background-color:#ccc;}*/
    a.delete-task,a.delete-story{width:9px;height:9px;background:transparent url(/images/the-master-delete-box.gif) no-repeat 0 0;border:1px solid #999;padding:0;margin:0;text-indent:-999999px;}
    table#scrumy-board td.stories{
        padding:16px 10px;
        width:157px;
    }
    div.story,.priority_section .new_story_link{position:relative;font-size:12px;background-color:#f7f7f7;border:1px solid #f0f0f0;-moz-border-radius:2px;-webkit-border-radius:2px;border-radius:2px;}div.story .title{word-wrap:break-word;}div.story a.edit-story{display:none;position:absolute;bottom:1px;right:1px;}div.story:hover a.edit-story{display:block;}div.story a.delete-story{display:none;position:absolute;top:1px;right:1px;overflow:hidden;}div.story:hover a.delete-story{display:block;}

    div.story .story-handle,table#scrumy-board th.story-handle{cursor:move;}
    .readonly div.story .story-handle,.readonly table#scrumy-board th.story-handle{cursor:default;}div.story .story-handle,.priority_section .story_editor form.story_editor{min-height:50px;width:145px;padding:5px;}#new_story_editor form.story_editor{padding:5px;}.mac .task_assignment_box,.mac #task_editor_buttons input{font-family:Lucida Grande;}.mac .task_assignment_box #task_scrumer_name,.mac .task_assignment_box .auto-complete{font-size:12px;}.mac #task_editor_buttons input{font-size:11px;}.windows_6 .task_assignment_box input{font-family:Arial;}.windows_6 #task_editor_buttons input{font-family:Segoe UI;padding-bottom:1px;}.windows_6 .task_assignment_box input,.windows_6 #task_editor_buttons input{font-size:12px;}.windows_5 #task_editor_buttons input{font-family:Tahoma;font-size:12px;}.windows_5 #task_editor_buttons input{font-family:Arial;font-size:12px;}.windows_5 #task_editor_buttons input{padding-bottom:1px;}.auto-complete{color:#000;width:350px;background:#fff;font-size:12px;z-index:1;}.auto-complete ul{border:1px solid #888;margin:0;padding:0;list-style-type:none;}.auto-complete ul li{margin:0;padding:3px;opacity:.6;filter:alpha(opacity=60);background-image:none;cursor:default;}.auto-complete ul li.selected{opacity:1;filter:alpha(opacity=100);}

    pre.debug{margin-top:25px;padding:5px;background-color:#eee;font-size:11px;clear:both;white-space:pre-line;}#tooltip{position:absolute;text-align:center;padding:3px;z-index:1000;background-color:white;border:1px solid gray;font-size:11px;max-width:150px;}#task_editor{background-image:url(/images/taskeditor-shadow.png);z-index:100;position:absolute;width:270px;height:253px;}#task_editor_box{border:solid 5px #222;border-bottom-width:30px;-moz-border-radius:4px;-webkit-border-radius:4px;border-radius:4px;height:169px;width:200px;left:25px;top:14px;position:relative;z-index:102;}.task_assignment_box{float:left;}.task_assignment_box input{background-color:inherit;border:1px inset;color:inherit;height:15px;line-height:1;margin:0 2px 0 0;padding:2px;width:157px;}.task_assignment_icon{bottom:5px;left:7px;position:absolute;}#task_editor_cancel_area{position:fixed;top:0;left:0;width:100%;height:100%;z-index:101;}.dk.save_button,.dk.cancel_button,#task_editor .scrumer_selector{display:block;float:left;overflow:hidden;margin:0 0 0 3px;background-position:0 0;background-repeat:no-repeat;background-color:transparent;outline:none;border:0;}.dk.save_button,.dk.cancel_button{border:1px solid;}#task_editor .save_button,#task_editor .cancel_button{height:21px;}.mac #task_editor .save_button,.mac #task_editor .cancel_button{float:right;}#task_editor .save_button{width:60px;background-position:-59px -2px;color:#77E592;padding-bottom:2px;padding-right:7px;text-align:right;}#task_editor .save_button::-moz-focus-inner,#task_editor .cancel_button::-moz-focus-inner{border:none;}#task_editor .save_button:active{background-position:-59px -26px;color:#477552;padding-bottom:0;}#task_editor .save_button.highlight{border-color:#475;}#task_editor .cancel_button{height:21px;width:55px;padding-bottom:2px;text-align:center;}#task_editor .cancel_button:active{background-position:-70px -26px;color:#777;padding-bottom:0;}#task_editor_buttons{bottom:-25px;position:absolute;right:0;}.story_editor .save_button,.story_editor .cancel_button{height:18px;background-position:center -3px;}.dk.save_button{border-color:#333;background-image:url(/images/task-editor--save.png);}button::-moz-focus-inner{border:0;}.story_editor .save_button{width:92px;}.story .save_button{margin:3px 0 0 0;width:108px;}.story .cancel_button{margin:3px -7px 0 2px;}.dk.cancel_button{border-color:#333;color:#ccc;background-image:url(/images/task-editor--cancel.png);cursor:pointer;}.story_editor .cancel_button{height:20px;width:33px;}#task_editor .scrumer_selector{height:20px;width:28px;background-image:url(/images/scrumer-selector.png);background-position:0 -3px;}.story_editor .save_button:active,.story_editor .cancel_button:active{background-position:center -28px;}#task_editor .scrumer_selector:active{background-position:center -28px;}#task_editor .scrumer_selector:active,#task_editor .scrumer_selector:focus,#task_editor .scrumer_selector::-moz-focus-inner{outline:none;border:none;}.dk.save_button[disabled],.dk.cancel_button[disabled],#task_editor .scrumer_selector[disabled]{opacity:.66;filter:alpha(opacity=66);cursor:default;}#task_editor .save_button[disabled]{background-position:-59px -52px;}#task_editor .cancel_button[disabled]{background-position:-70px -52px;}#task_editor .save_button[disabled],#task_editor .cancel_button[disabled]{color:#b5b5b5;}#task_editor .save_button[disabled]:active,#task_editor .cancel_button[disabled]:active{padding-bottom:2px;}#task_editor .save_button[disabled].highlight{border-color:#333;}.story_editor .save_button[disabled],.story_editor .cancel_button[disabled]{background-position:center -53px;}#task_editor .scrumer_selector[disabled]{background-position:center -50px;}textarea[readOnly],input[type=text][readOnly]{color:#999;}#scrumy-board .edit-story-link{visibility:hidden;}.saving{color:#999;margin-left:2px;}.story_editor{text-align:center;font-size:13px;}.story textarea{margin-top:-1px;width:143px;resize:none;}.move_to_story_form{display:inline-block;}.story-import{color:#0046B6;text-transform:uppercase;}.story_editor textarea{font-family:Arial;font-size:12px;overflow-x:hidden;text-align:inherit;}a#new_story_link,a.new_story_link{font-weight:normal;font-size:12px;color:#999;}a#new_story_link:hover,a.new_story_link:hover{color:#333;}#scrumy-board .new_story_row{height:50px;position:relative;z-index:0;top:0;left:0;}#scrumy-board .new_story_row td{border-top:1px solid #f0f0f0;background-color:#fff;}#scrumy-board #new_story_box{text-align:center;}#scrumy-board #new_story_box #new_story_link{display:block;padding:18px;}#indicator{width:100%;text-align:center;position:fixed;top:3px;z-index:200;}#indicator span{color:#fff;background-color:#000;border:1px solid #000;border-radius:0 0 3px 3px;-moz-border-radius:0 0 3px 3px;-webkit-border-top-left-radius:0;-webkit-border-top-right-radius:0;-webkit-border-bottom-left-radius:3px;-webkit-border-bottom-right-radius:3px;font-size:12px;font-weight:bold;padding:2px 12px;}#indicator span.error-indicator{background-color:#FAB;border:1px solid #FA98AA;color:#000;}#indicator a{color:#fff;}#indicator .error-indicator a{color:#400;}#row-loading-indicator{color:#666;font-size:14px;padding-top:30px;position:absolute;text-align:center;}#placeholder-row{height:82px;opacity:.5;filter:alpha(opacity=50);}#dropmarker{border-color:#999;border-style:solid;border-width:0 2px;height:4px;margin-left:10px;padding:2px 0 0;width:155px;}#dropmarker hr{height:2px;border:none;background-color:#999;}.demo_popup .caption{font-size:12px;margin-right:5px;}.demo_popup div,.demo_popup form{display:inline;}.windows_6 .demo_popup form input{font-family:Segoe UI;font-size:12px;padding-left:8px;padding-right:8px;*padding-left:5px;*padding-right:5px;*height:23px;}.windows_5 .demo_popup form input{font-family:Tahoma;font-size:11px;padding-top:1px;padding-left:8px;padding-right:8px;padding-bottom:1px;margin-right:2px;*margin-right:0;}.demo_popup{-moz-border-radius:3px;-webkit-border-radius:3px;border-radius:3px;background-color:#eee;margin-bottom:auto;margin-left:170px;margin-right:0;margin-top:auto;padding-bottom:3px;padding-left:10px;padding-right:7px;padding-top:4px;position:absolute;}div.glowstick{background:url(/images/glowstix.png) no-repeat -2px -4px transparent;display:block;height:77px;}div.glowstick.red{background-position:-94px -4px;}td.glowcell{background-color:#eefff5!important;}div.glowstory{background-color:#6efede;border-color:#40ddba;}.hidden{display:none;}

    body{background:url(/images/pro/scrumypro-login-bg.png) repeat;}body.free_popup{background:url(/images/carlton-dancing.gif) repeat;}.popup{position:fixed;top:0;width:100%;z-index:1002;color:#333;}.mac .popup{font-family:"Lucida Grande";font-size:13px;}.windows_6 .popup{font-family:Segoe UI;font-size:12px;}.windows_5 .popup{font-family:Tahoma;font-size:11px;}.ios .popup{font-family:Arial;font-size:13px;}.popup h1{color:#222;display:block;font-family:Rockwell,Arial;font-size:22px;letter-spacing:-2px;line-height:1em;margin:9px 0;}a{color:#666;}.overlay{position:fixed;display:block;background-color:#000;top:0;left:0;width:100%;height:100%;opacity:.5;filter:alpha(opacity = 50);z-index:1001;}.popup .buttons{clear:both;float:right;padding-bottom:15px;padding-top:15px;*width:138px;}.mac .popup .bt-ok{float:right;}.mac .popup .bt-cancel{float:left;}.mac .popup .buttons input,.mac .popup .buttons button{font-family:inherit;font-size:inherit;margin-left:10px;width:70px;}.windows_6 .popup .bt-ok{float:left;}.windows_6 .popup .bt-cancel{float:right;}.windows_6 .popup .buttons .bt-ok{*padding-left:8px;*padding-right:8px;}.windows_6 .popup .buttons{*width:117px;}.windows_6 .popup #settings .buttons{*width:124px;margin-left:-1px;}.windows_6 .popup #feedback .buttons{*width:132px;}.windows_5 .popup .buttons{*width:166px;}.windows_6 .popup .buttons input,.windows_6 .popup .buttons button,.windows_6 #settings .info_bar input{font-family:Segoe UI;font-size:12px;padding-left:10px;padding-right:10px;*padding-left:5px;*padding-right:5px;*height:23px;}.windows_5 .popup .bt-ok{float:left;}.windows_5 .popup .bt-cancel{float:right;}.windows_5 .popup .buttons input,.windows_5 .popup .buttons button{font-family:Tahoma;font-size:11px;padding-top:1px;padding-left:10px;padding-right:10px;padding-bottom:1px;margin-right:6px;*margin-right:0;width:75px;}.windows_5 #settings .info_bar input{font-family:Tahoma;}.mac .popup #pw_label,.mac .popup #pw_field{height:16px;}.windows_6 .popup #pw_label,.windows_6 .popup #pw_field{height:17px;}.windows_5 .popup #pw_label,.windows_5 .popup #pw_field{font-size:11px;height:15px;}#feedback textarea{padding:4px;}.mac #feedback textarea{width:295px;}.windows_6 #feedback textarea,.windows_5 #feedback textarea{width:292px;}.popup #feedback .notice{margin:10px 0;}.popup table.layout #feedback input.field{margin:5px 0;width:98%;}.windows_6 .popup input.field{font-family:Segoe UI;font-size:12px;}.windows_6 .popup input.field,.windows_5 .popup input.field{padding:2px;}.mac .popup input.field{padding:1px;}.popup table.layout #feedback label{display:inline-block;}.popup input.field{margin-bottom:10px;}.popup input.field.bottom{margin-bottom:0;}.popup textarea{font:inherit;}
    .popup label{vertical-align:baseline;width:72px;}.popup .label{vertical-align:text-top;font-size:inherit;width:40px;}.popup input.field{font:inherit;margin-left:10px;}.popup p.notice{margin-top:0;margin-bottom:15px;padding:0;font-weight:bold;clear:both;}.mac .popup p{margin-bottom:20px;}.popup p.error{color:#f00;}#signin .password_form_field,#forgot-password .password_form_field,#forgot-password .password_reset_table tr,#settings .password_form_field{white-space:nowrap;}#inaccessible{padding-right:5px;padding-left:5px;padding-bottom:35px;padding-top:14px;width:500px;}
    #inaccessible label.label{vertical-align:top;width:230px;padding-bottom:10px;}#inaccessible .restore_button,#inaccessible .downgrade_button,#inaccessible .cancel_button{-moz-border-radius:5px;-webkit-border-radius:5px;display:block;margin:-5px 0 10px;padding:10px 20px;text-decoration:none;background-color:#f5f5f5;border:1px solid #ccc;outline:none;}#inaccessible .restore_button{color:#08a;font-size:12px;padding-bottom:20px;padding-top:15px;}#inaccessible .downgrade_button,#inaccessible .cancel_button{width:205px;font-size:10px;height:90px;}#inaccessible .downgrade_button{float:left;}#inaccessible .cancel_button{float:right;}#inaccessible .button_title{font-family:Arial;display:block;font-weight:bold;margin-bottom:5px;}
    #inaccessible .restore_button .button_title{font-size:25px;}#inaccessible .downgrade_button .button_title,#inaccessible .cancel_button .button_title{font-size:16px;}#inaccessible .restore_button:hover{color:#044;background-color:#eee;border-color:#ccc;}#inaccessible .downgrade_button:hover{color:#333;background-color:#eee;border-color:#ccc;}#inaccessible .cancel_button:hover{color:#900;background-color:#eee;border-color:#ccc;}#inaccessible .restore_button:active,#inaccessible .downgrade_button:active,#inaccessible .cancel_button:active{background-color:#ccc;border-color:#aaa;outline:none;}#inaccessible .downgrade_button:active,#inaccessible .cancel_button:active{padding-top:11px;padding-bottom:9px;}#inaccessible .restore_button:active{padding-top:16px;padding-bottom:19px;}#inaccessible li{font-size:11px;list-style:none;}#inaccessible ul{margin:0 -1em 10px;}#inaccessible form{white-space:normal;}#inaccessible form .field_label{white-space:nowrap;}#inaccessible .signout{margin:20px 0 0 0;}#inaccessible #settings .password_grp{clear:none;margin-left:3px;}#forgot-password{padding-left:5px;padding-bottom:35px;padding-top:14px;width:auto;*width:331px;}#forgot-password .password_reset_table td{text-align:right;}.windows_5 .popup a.forgot-password{font-size:9px;top:20px;*top:18px;}.popup table.layout{border-collapse:collapse;top:0;}.popup table.layout td{padding:0;margin:0;}.popup table.layout .left-edge{background:url(/images/pro/popup-l.png) no-repeat right bottom;width:50%;}.popup table.layout .middle{background:url(/images/pro/popup-m.png) repeat-x bottom;padding-bottom:15px;}.popup table.layout .right-edge{background:url(/images/pro/popup-r.png) no-repeat left bottom;width:50%;}

    #tutorial{position:absolute;padding:10px 15px;max-width:300px;border:1px solid #9c9c9c;background-color:#f0f7ff;color:#333;-moz-border-radius:5px;-webkit-border-radius:5px;border-radius:5px;font-size:13px;}#tutorial p{padding:3px 0;}

    #lobby #site_nav,#a-content #site_nav,#c-content #site_nav{margin-bottom:35px;}#lobby #site_nav a,#a-content #site_nav a,#c-content #site_nav a{font-size:12px;font-weight:bold;letter-spacing:-1px;text-transform:uppercase;margin-right:15px;}#lobby #site_nav a,#a-content #site_nav a,#c-content #site_nav a{text-decoration:none;}#lobby #site_nav a:hover,#a-content #site_nav a:hover,#c-content #site_nav a:hover{text-decoration:underline;}#lobby #site_nav a{color:#999;}#lobby #site_nav a[href='/']{color:#d8d8d8;}#a-content #site_nav a{color:#000;}#a-content #site_nav a[href='/about']{color:#ccc;}#c-content #site_nav a{color:#222;}#c-content #site_nav a[href='/contact']{color:#ccc;}#footer #site_nav{float:right;}
        table {
            /*border: solid 1px #ccc;*/
            
            border-collapse: inherit;
            border-spacing: 1px;
            border-color: gray;
        }
    table#scrumy-board{border-color:#ccc!important;}
    table#scrumy-board th,table#scrumy-board td{border-color:#ccc!important;}#footer,#footer a{color:#999;}#footer img{opacity:1;filter:alpha(opacity=100);}


    td.status_1{
        background-color: #FCFCFC;
    }
    
    td.status_2{
        background-color: #F4E1C3;
    }
    
    td.status_3{
        background-color: #FFC2C2;
    }
    
    td.status_5{
        background-color: #BCFABC;
    }
    
</style>
<br>
<div class="row">
    <div class="col-md-12">
<!--        <div class="page-header">
            <h1 id="forms">Ciclo</h1>
        </div>-->
        <div class="well">
            <form id="form-search" name="form-search" method="post" class="form-horizontal">
                <fieldset>
                    <div class="form-group">
                        <legend>Definir Ciclo</legend>
                    </div>
                    <div class="form-group">
                        <label for="inputEmail" class="col-lg-2 control-label" for="Programa">Selecione um programa</label>
                        <div class="col-lg-10">
                            <?php
                            $sql = 'SELECT prg.prgid AS codigo, prg.prgdsc AS descricao FROM scrum.programa prg';
                            $db->monta_combo(
                                    'prgid', $sql, 'S', 'Selecione..', 'search', '', null, null, 'N', 'prgid', null, $_POST['prgid'], null, 'class="form-control" style="width=100%;"'
                            );
                            ?>
                        </div>
                        <!--<p class="help-block">Selecione um programa.</p>-->
                    </div>
                    <div class="form-group">
                        <label for="inputEmail" class="col-lg-2 control-label" for="Programa">Selecione um ciclo</label>
                        <div class="col-lg-10">
                            <?php
                            $sqlSprint = "SELECT sptid as codigo, 
                                            (TO_CHAR(sptinicio, 'DD/MM/YYYY') || ' - ' || TO_CHAR(sptfim, 'DD/MM/YYYY'))  as descricao
                                            FROM scrum.sprint
                                            ORDER BY sptinicio , sptfim";
                            $db->monta_combo(
                                    'sptid', $sqlSprint, 'S', 'Selecione..', 'search', '', null, null, 'N', 'sptid', null, $idSprintAtual, null, 'class="chosen-select form-control" style="width=100%;"'
                            );
                            ?>
                        </div>
                        <!--<p class="help-block">Selecione um programa.</p>-->
                    </div>
                </fieldset>
            </form>
            <fieldset>
                    <div>
                        <table id="scrumy-board" cellspacing="0" class="live ">
                            <tbody id="scrumy-board-body">
                                <tr id="scrumy-board-header"  style="position: relative;">
<!--                                    <th id="story-header" >
                                        <span>Subprograma</span>
                                    </th>
                                    <th id="story-header">
                                        <span>Estória</span>
                                    </th>
                                    <th id="plus-header">
                                    </th>-->
                                    <th id="header-sprintAnterior" class="header">
                                        <span>Ciclo anterior</span> <br /> 
                                        <?php echo $sprintAnterior['sptinicio'] ?> - <?php echo $sprintAnterior['sptfim'] ?> <br />
                                        (<span class="sprintHorasGastas"><?php echo $sprintAnteriorHorasGastas ?>hs</span>/ <?php echo $programa['prghrsprint'] ?>hs )
                                        <a href="#" class="select-all" style="display: none">Select All</a>
                                    </th>
                                    <th id="header-sprintAtual" class="header">
                                        <span>Ciclo atual</span> <br /> 
                                        <?php echo $sprintAtual['sptinicio'] ?> - <?php echo $sprintAtual['sptfim'] ?> <br />
                                        (<span class="sprintHorasGastas"><?php echo $sprintAtualHorasGastas ?>hs</span>/ <?php echo $programa['prghrsprint'] ?>hs )
                                        <a href="#" class="select-all" style="display: none">Select All</a>
                                    </th>
                                    <th id="header-proximaSprint" class="header">
                                        <span>Próximo ciclo</span> <br /> 
                                        <?php echo $proximaSprint['sptinicio'] ?> - <?php echo $proximaSprint['sptfim'] ?> <br />
                                        (<span class="sprintHorasGastas"><?php echo $proximaSprintHorasGastas ?>hs</span>/ <?php echo $programa['prghrsprint'] ?>hs )
                                        <a href="#" class="select-all" style="display: none">Select All</a>
                                    </th>
                                    <th id="header-backLog" class="header">
                                        <span>Backlog</span>
                                        <a href="#" class="select-all" style="display: none">Select All</a>
                                    </th>
                                </tr>
                                <tr class="story-task-row">
                                    <td id="sprintAnterior" class="linha status_3" sprintId="<?php echo $idSprintAnterior ?>" sprintHorasGastas="<?php echo $sprintAnteriorHorasGastas ?>" sprintHoras="<?php echo $programa['prghrsprint'] ?>">
                                        <?php if($entregaveisSprintAnterior): foreach ( $entregaveisSprintAnterior as $entregavelSprintAnterior ): ?>
                                            <?php 
                                                if($entregavelSprintAnterior['usucpfresp_dsc']){
                                                    $nomeResponsavel = explode(' ', $entregavelSprintAnterior['usucpfresp_dsc']);
                                                    $nomeResponsavel = $nomeResponsavel[0];
                                                } else {
                                                    $nomeResponsavel = '';
                                                } 
                                            ?>
                                        
                                            <div sprintPostitId="<?php echo $entregavelSprintAnterior['entid'] ?>" sprintPostitHoras="<?php echo $entregavelSprintAnterior['enthrsexec'] ?>" class="task" style="background-color: <?php echo $entregavelSprintAnterior['subprgcolor'] ?>">
                                                <div class="handle"> <?php echo $entregavelSprintAnterior['subprgdsc']//$itemStatus['enthrsexec'] ?> </div>
                                                <div class="title" >
                                                    <pre class="title_inner">
                                                        <b><?php echo $entregavelSprintAnterior['esttitulo'] ?></b> 
                                                        <?php echo $entregavelSprintAnterior['entdsc'] ?>
                                                        
                                                    </pre>
                                                </div>
                                                <div class="iconEdit" sprintPostitId="<?php echo $entregavelSprintAnterior['entid'] ?>">
                                                    <i class="glyphicon glyphicon-pencil"></i>
                                                </div>
                                                <!--unassigned-->
                                                <div class="assignment">
                                                    <?php echo $entregavelSprintAnterior['enthrsexec'] ?>hs
                                                </div>

                                                <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                                <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                                <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                            </div>
                                            <?php // ver($entregavelBackLog); ?>
                                        <?php endforeach; endif;?>
                                    </td>
                                    <td id="sprintAtual" class="linha status_5" sprintId="<?php echo $idSprintAtual ?>" sprintHorasGastas="<?php echo $sprintAtualHorasGastas ?>" sprintHoras = "<?php echo $programa['prghrsprint'] ?>">
                                        <?php if($entregaveisSprintAtual): foreach ( $entregaveisSprintAtual as $entregavelSprintAtual ): ?>
                                            <?php 
                                                if($entregavelSprintAtual['usucpfresp_dsc']){
                                                    $nomeResponsavel = explode(' ', $entregavelSprintAtual['usucpfresp_dsc']);
                                                    $nomeResponsavel = $nomeResponsavel[0];
                                                } else {
                                                    $nomeResponsavel = '';
                                                } 
                                            ?>
                                        
                                            <div sprintPostitId="<?php echo $entregavelSprintAtual['entid'] ?>" sprintPostitHoras="<?php echo $entregavelSprintAtual['enthrsexec'] ?>" class="task" style="background-color: <?php echo $entregavelSprintAtual['subprgcolor'] ?>">
                                                <div class="handle"> <?php echo $entregavelSprintAtual['subprgdsc']//$itemStatus['enthrsexec'] ?> </div>
                                                <div class="title" >
                                                    <pre class="title_inner">
                                                        <b><?php echo $entregavelSprintAtual['esttitulo'] ?></b> 
                                                        <?php echo $entregavelSprintAtual['entdsc'] ?>
                                                        
                                                    </pre>
                                                </div>
                                                <div class="iconEdit" sprintPostitId="<?php echo $entregavelSprintAtual['entid'] ?>">
                                                    <i class="glyphicon glyphicon-pencil"></i>
                                                </div>
                                                <!--unassigned-->
                                                <div class="assignment">
                                                    <?php echo $entregavelSprintAtual['enthrsexec'] ?>hs
                                                </div>

                                                <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                                <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                                <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                            </div>
                                            <?php // ver($entregavelBackLog); ?>
                                        <?php endforeach; endif; ?>
                                    </td>
                                    <td id="proximaSprint" class="linha status_2"  sprintId = "<?php echo $idProximaSprint ?>" sprintHorasGastas="<?php echo $proximaSprintHorasGastas ?>" sprintHoras = "<?php echo $programa['prghrsprint'] ?>">
                                        <?php if($entregaveisProximaSprint): foreach ( $entregaveisProximaSprint as $entregavelProximaSprint ): ?>
                                            <?php 
                                                if($entregavelProximaSprint['usucpfresp_dsc']){
                                                    $nomeResponsavel = explode(' ', $entregavelProximaSprint['usucpfresp_dsc']);
                                                    $nomeResponsavel = $nomeResponsavel[0];
                                                } else {
                                                    $nomeResponsavel = '';
                                                } 
                                            ?>
                                        
                                            <div sprintPostitId="<?php echo $entregavelProximaSprint['entid'] ?>" sprintPostitHoras="<?php echo $entregavelProximaSprint['enthrsexec'] ?>" class="task" style="background-color: <?php echo $entregavelProximaSprint['subprgcolor'] ?>">
                                                <div class="handle"> <?php echo $entregavelProximaSprint['subprgdsc']//$itemStatus['enthrsexec'] ?> </div>
                                                <div class="title" >
                                                    <pre class="title_inner">
                                                        <b><?php echo $entregavelProximaSprint['esttitulo'] ?></b> 
                                                        <?php echo $entregavelProximaSprint['entdsc'] ?>
                                                        
                                                    </pre>
                                                </div>
                                                <!--unassigned-->
                                                <div class="iconEdit" sprintPostitId="<?php echo $entregavelProximaSprint['entid'] ?>">
                                                    <i class="glyphicon glyphicon-pencil"></i>
                                                </div>
                                                <div class="assignment">
                                                    <?php echo $entregavelProximaSprint['enthrsexec'] ?>hs
                                                </div>
                                                <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                                <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                                <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                            </div>
                                            <?php // ver($entregavelBackLog); ?>
                                        <?php endforeach; endif;?>
                                    </td>
                                    <td id="backLog" class="linha status_1" sprintId="" sprintHorasGastas="" sprintHorasLimite="">
                                        <?php if($entregaveisBackLog): foreach ( $entregaveisBackLog as $entregavelBackLog ): ?>
                                            <?php 
                                                if($entregavelBackLog['usucpfresp_dsc']){
                                                    $nomeResponsavel = explode(' ', $entregavelBackLog['usucpfresp_dsc']);
                                                    $nomeResponsavel = $nomeResponsavel[0];
                                                } else {
                                                    $nomeResponsavel = '';
                                                } 
                                            ?>
                                        <!--background-color: rgba(255, 0, 0, 0.5)-->
                                            <div sprintPostitId="<?php echo $entregavelBackLog['entid'] ?>" sprintPostitHoras="<?php echo $entregavelBackLog['enthrsexec'] ?>" class="task" style="background-color: <?php echo $entregavelBackLog['subprgcolor'] ?>">
                                                <div class="handle"> <?php echo $entregavelBackLog['subprgdsc']//$itemStatus['enthrsexec'] ?> </div>
                                                <div class="title" >
                                                    <pre class="title_inner">
                                                        <b><?php echo $entregavelBackLog['esttitulo'] ?></b> 
                                                        <?php echo $entregavelBackLog['entdsc'] ?>
                                                    </pre>
                                                </div>
                                                <div class="iconEdit" sprintPostitId="<?php echo $entregavelBackLog['entid'] ?>">
                                                    <i class="glyphicon glyphicon-pencil"></i>
                                                </div>
                                                <!--unassigned-->
                                                <div class="assignment">
                                                    <?php echo $entregavelBackLog['enthrsexec'] ?>hs
                                                </div>
                                                <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                                <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                                <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                            </div>
                                            <?php // ver($entregavelBackLog); ?>
                                        <?php endforeach; endif;?>
                                    </td>
                                </tr>
                        </tbody>
                    </table>
                    <br />
                    <br />
                    <br />
    </div>
</div>
<script lang="javascript">
    
    $('.iconEdit').click(function(){
        $.post( window.location.href, {'controller': 'postit' , 'action' : 'formularioModal', 'entid' : $(this).attr('sprintPostitId')},  function(html) {
            $('#modal').html(html).modal('show');
        });
    });
    
    $(function() {
        $( "#sprintAnterior , #sprintAtual , #proximaSprint , #backLog" ).sortable({
          connectWith: ".linha",
            beforeStop: function( event, ui ) {
                
                var column = $(ui.item.context).parent('td');
                var columnId = column.attr('id');
                var postit = $(ui.item.context);
                
                var sprintId = column.attr('sprintId');
                var sprintHoras = column.attr('sprintHoras');
                var sprintHorasGastas = column.attr('sprintHorasGastas');
                var sprintPostitId = postit.attr('sprintPostitId');
//                console.info(sprintId);
                // Pegando os IDs dos postits que estao na coluna que foi inclusa o postit
                var arrColumnPostit = column.children('div .task');
                var sprintArrPostitId = new Array(0);
                sprintHorasGastasNovo = 0;
                n = 0;
                arrColumnPostit.each(function( i , element){
                    if($(element).attr('sprintPostitId')){
                        sprintArrPostitId[n] = $(element).attr('sprintPostitId');
                        sprintHorasGastasNovo += parseInt( $(element).attr('sprintPostitHoras'));
                        n++;
                    }
                });
                
                var isValid = true;
                if($(event.target).attr('id') != columnId){
                    if(columnId != 'backLog' && parseInt(sprintHorasGastasNovo) > parseInt(sprintHoras)){
                        $( "#"+ columnId + ' , #' +$(event.target).attr('id')).sortable('cancel');
                        isValid = false;
                    } else {
                        
                        // Alterando o html th da coluna onde o postit esta.
                        $('#header-' + columnId).children('.sprintHorasGastas').html(sprintHorasGastasNovo + 'hs');
//                        console.info(sprintHorasGastasNovo);
                        
                        // Alterando o html th da coluna de  onde o postit veio.
                        sprintAnteriorHorasGastasNovo = 0;
                        $(event.target).children('div .task').each(function( i , element){
                            sprintAnteriorHorasGastasNovo += parseInt( $(element).attr('sprintPostitHoras'));
                        });
                        $('#header-' + $(event.target).attr('id')).children('.sprintHorasGastas').html(sprintAnteriorHorasGastasNovo + 'hs');
                    }
                }
                
                if(isValid){
                console.info(sprintArrPostitId);
                    dataPost = {action: 'alterarSprint', idpostit : sprintPostitId, idsprint : sprintId, arrPostitId : sprintArrPostitId}
                    $.post( window.location, dataPost, function(result) {
                        console.log( result );
                    });
                    
//                    console.info(dataPost);
                } else {
                    $("body").animate({ scrollTop: $('.aviso').height() }, "slow"
                    , function(){
                        $('.aviso').hide().empty().append('<div class="alert alert-dismissable alert-warning" ><button class="close" data-dismiss="alert" type="button">×</button><strong>Aviso!</strong><a class="alert-link" href="#"> A quantidade de horas do entregavel ultrapassa o limite do sprint, </a> retire um entregável para diminuir a quantidade de horas e tente novamente.</div></div>').fadeIn();
                    });
                    
                    console.info('Tem muitas horas champs');
                }
             }
        }).disableSelection();
        
        
  });
    
    function search()
    {
        $('#form-search').submit();
    }
</script>