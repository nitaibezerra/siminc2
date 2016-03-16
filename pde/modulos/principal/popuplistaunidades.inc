<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=9" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

    <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <script language="JavaScript" src="../includes/funcoes.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
    <script>
        atualizaUsuario();
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')}
    </style>
</head>
<body>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div>
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <div style="float:right;cursor:pointer;" onclick="window.close();">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Lista de Unidades</div></td>
    </tr>
    <?php
    $titulo = $_REQUEST['parametro'];
    ?>
    <tr>
        <td class="fundo_padrao" colspan="3">
            <div>
                <img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" ><?=$titulo?><br/> </div>
            </div>
            <?php
            if(substr($titulo,0,4)=='Luiz'){
                $sql = "SELECT DISTINCT
                            dshuf AS cutuf,
                            entinst.entsig AS cutinstituto,
                            entcampus.entnome AS cutunidade
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid AND entcampus.entstatus = 'A'
                        INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.fuestatus = 'A'
                        inner join entidade.funentassoc a on a.fueid = fn.fueid
                        inner join entidade.entidade entinst on entinst.entid = a.entid
                        WHERE sh.indid IN (525)
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                        AND dsh.tidid1 IN (2257)
                        AND sh.sehstatus <> 'I'
                        ORDER BY cutuf, cutinstituto, cutunidade";
            }elseif(substr($titulo,0,5)=='Dilma'){
                $sql = "SELECT DISTINCT
                            dshuf AS cutuf,
                            entinst.entsig AS cutinstituto,
                            entcampus.entnome AS cutunidade
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid AND entcampus.entstatus = 'A'
                        INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.fuestatus = 'A'
                        inner join entidade.funentassoc a on a.fueid = fn.fueid
                        inner join entidade.entidade entinst on entinst.entid = a.entid
                        WHERE sh.indid IN (525)
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                        AND dsh.tidid1 IN (2596, 2597)
                        AND sh.sehstatus <> 'I'
                        ORDER BY cutuf, cutinstituto, cutunidade";
            }else{
                $sql = "SELECT cutuf, cutinstituto, cutunidade
                        FROM pde.cockpit_unidades_tempo
                        WHERE cutpresidente ILIKE '%".substr(trim($titulo), 0, 10)."%'
                        ORDER BY cutuf, cutinstituto, cutunidade";
            }
            $arrDados = $db->carregar( $sql );
            if($arrDados){
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Instituto</th>
                    <th>Unidade</td>
                    <th>UF</td>
                </tr>
                <?php
                $count=-1;
                foreach($arrDados as $dado):
                    $count++;
                    ?>
                    <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                        <td class="" ><?php echo $dado['cutinstituto'] ?></td>
                        <td class="" ><?php echo $dado['cutunidade'] ?></td>
                        <td class="" ><?php echo $dado['cutuf'] ?></td>
                    </tr>
                <?php
                endforeach;
                ?>
            </table>
            <?php
            }
            ?>
        </td>
    </tr>
</table>
</body>
</html>