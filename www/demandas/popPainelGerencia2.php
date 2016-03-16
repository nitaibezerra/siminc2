<?php
/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 02/09/2013 por Orion Teles <orionteles@gmail.com>
 */


// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if ($_REQUEST['atualizar_pdeinterativo_query']) {
	
	if(strstr( $_SERVER['SERVER_NAME'], 'simec-local') || strstr( $_SERVER['SERVER_NAME'], 'simec-d')){
	    echo 10;
	    die;
	}
	
	$sql = "select procpid
			From dblink('host= user= password= port= dbname=',
			'select procpid
			            from pg_stat_activity
			            where current_query not like ''%IDLE%''
			            ') 
			AS pd (
			procpid integer
			)";
	
	$dados = $db->carregar($sql);
	
	

	echo count($dados) > 100 ? 100 : count($dados);
	die;
}

if ($_REQUEST['atualizar_query']) {
	
    $sql = "select datname, pid, usename, query, waiting,
            client_addr, (now() - backend_start) as tempo_backend, (now() - query_start) as tempo_query,
            ((substring(replace((now() - query_start)::interval::varchar,':',''),1,2)::integer*3600)+(substring(replace((now() - query_start)::interval::varchar,':',''),3,2)::integer*60)+ (substring(replace((now() - query_start)::interval::varchar,':',''),5,9)::float)) dur_segundos
            from pg_stat_activity
            where query not like '%IDLE%'
            order by tempo_query desc";
    $dados = $db->carregar($sql);

    echo count($dados) > 100 ? 100 : count($dados);
    die;
}

if ($_REQUEST['atualizar_query_tempo']) {
    $sql = "select
            sum(((substring(replace((now() - query_start)::interval::varchar,':',''),1,2)::integer*3600)+(substring(replace((now() - query_start)::interval::varchar,':',''),3,2)::integer*60)+ (substring(replace((now() - query_start)::interval::varchar,':',''),5,9)::float))) dur_segundos
            from pg_stat_activity
            where current_query not like '%IDLE%'
            and current_query not ilike '%COPY%'
            and current_query not ilike '%VACUUM%'
            ";
    $dados = $db->pegaUm($sql);

    echo $dados > 1000 ? 1000 : $dados;
    die;
}

if ($_REQUEST['atualizar_pdeinterativo_query_tempo']) {
	
	if(strstr( $_SERVER['SERVER_NAME'], 'simec-local') || strstr( $_SERVER['SERVER_NAME'], 'simec-d')){
		echo 100;
		die;
	}
	
	$sql = "select dur_segundos
			From dblink('host= user= password= port= dbname=',
			'select
			            coalesce(sum(((substring(replace((now() - query_start)::interval::varchar,'':'',''''),1,2)::integer*3600)+(substring(replace((now() - query_start)::interval::varchar,'':'',''''),3,2)::integer*60)+ (substring(replace((now() - query_start)::interval::varchar,'':'',''''),5,9)::float))),0) dur_segundos
			            from pg_stat_activity
			            where current_query not like ''%IDLE%''
			            and current_query not ilike ''%COPY%''
			            and current_query not ilike ''%VACUUM%''
			            ') 
			AS pd (
			dur_segundos double precision 
			)";
	$dados = $db->pegaUm($sql);

	echo $dados > 1000 ? 1000 : $dados;
	die;
}

if( $_REQUEST['useronline'] ){
    $sql = "select COALESCE(count(*),0) as usu_online
			from seguranca.usuariosonline
			";
    echo $db->pegaUm($sql);
    die;
}

if( $_REQUEST['useronline_pdeinterativo'] ){
	
	if(strstr( $_SERVER['SERVER_NAME'], 'simec-local') || strstr( $_SERVER['SERVER_NAME'], 'simec-d')){
		echo 100;
		die;
	}
	
	$sql = "select usu_online
			From dblink('host= user= password= port= dbname=',
			'select COALESCE(count(*),0) as usu_online
						from seguranca.usuariosonline
						            ')
						            AS pd (
						            usu_online integer
						            )
			";
	echo $db->pegaUm($sql);
	die;
}


if( $_REQUEST['modalUsuarios'] ){
    echo 1;
}

?>
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
<!--        <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" /> -->
        <meta content="IE=9" http-equiv="X-UA-Compatible" />

        <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

        <!-- Styles Boostrap -->
        <link href="/library/bootstrap-3.0.0/css/bootstrap.css" rel="stylesheet">

        <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet">
        <link href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">
        <link href="/library/bootstrap-modal-master/css/bootstrap-modal-bs3patch.css" rel="stylesheet" />
        <link href="/library/bootstrap-modal-master/css/bootstrap-modal.css" rel="stylesheet" />

        <!-- Custom Style -->
        <link href="/estrutura/temas/default/css/css_reset.css" rel="stylesheet">
        <link href="/estrutura/temas/default/css/estilo.css" rel="stylesheet">

        <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
        <!--[if lt IE 9]>
            <script src="/estrutura/js/html5shiv.js"></script>
        <![endif]-->
        <!--[if IE]>
            <link href="/estrutura/temas/default/css/styleie.css" rel="stylesheet">
        <![endif]-->

        <!-- Boostrap Scripts -->
        <script src="/library/jquery/jquery-1.10.2.js"></script>
        <script src="/library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
        <script src="/library/chosen-1.0.0/chosen.jquery.min.js"></script>
        <script src="/library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
        <script src="/library/bootstrap-modal-master/js/bootstrap-modalmanager.js"></script>
        <script src="/library/bootstrap-modal-master/js/bootstrap-modal.js"></script>


        <!-- Custom Scripts -->
        <script type="text/javascript" src="../includes/funcoes.js"></script>

        <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
        <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts-more.js"></script>
        <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

        <style type="text/css">

            .row {margin: 2px;}
            .box {padding: 5px; margin-bottom: 0px;}
            .box-principal {padding: 3px;}
            .panel { margin: 2px;
                border: 0px red;
                box-shadow: 0px 0px 4px 0px black;
            }

            .panel *{
                color: #fff;
            }

            .panel-heading{
                padding: 10px 2px;
            }

            .panel-body{
                color: #000;
                padding-top: 3px;
            }

			.panel-azul{ background: #0020C2; }
            .panel-atrasado{ background: #f00; }
            .panel-em-dia{ background: orange; }
            .panel-a-vencer{ background: green; }
            .panel-zerado{ background: #000; color: white; }

			.panel-body-azul{ background: #1E90FF;  }
            .panel-body-atrasado{ background: #FFEDED; }
            .panel-body-em-dia{ background: #fcf8e3; }
            .panel-body-a-vencer{ background: #dff0d8; }
            .panel-body-zerado{ background: #eee; }

            .panel h3{
                font-size: 11px !important;
                font-weight: bold;
                text-align: center;
                text-shadow: 0px 0px 4px #000;
            }
            
            .label-danger {background: red;}
            .label-warning {background: #FFA500; }
            .label-success {background: green;}
			.label-tabela {
            	font-size: 14px !important;
			    border-spacing: 6px;
			    font-weight: bold;
            }
            
			/*
			.label-tabela {
            	font-size: 14px !important;
            	border-collapse: separate;
			    border-spacing: 6px;
            	
            }
            */
            
            .box-green  {background: #0F6D39; padding: 20px;}

            .box-orange {background: #EE9200; padding: 20px;}

            tr.danger th,  tr.danger td  {background: #f2dede !important;}
            tr.warning th, tr.warning td {background: #fcf8e3 !important;}
            tr.success th, tr.success td {background: #dff0d8 !important;}

            .highcharts-container{
                margin: 0 !important;
            }
            
            .tituloGrafico{
            	text-align: center;
            	color: #fff;
            }
        </style>

        <script type="text/javascript">
        	$(function(){
        		setTimeout(function(){window.location.href = window.location.href;}, 50000);

        		$('.ver-detalhes').click(function(){
            		$('#div-entregas').load('popupDemandasPainel.php?usucpf='+$(this).attr('usucpf') );
        			$('#myModal').modal();
        		});

        		$('.ver-usuarios').click(function(){
            		$('#div-usuarios').load('popupGraficoUsuariosOnline.php?modalUsuarios=1');
        			$('#usuariosDetalhe').modal();
        		});
        	});
        </script>

    </head>

    <body>
        <!-- // Barra do Governo -->
        <?php //include_once "../barragoverno.php"; ?>

        <?php

        $_REQUEST['ordid']  = $_REQUEST['ordid']  ? $_REQUEST['ordid']  : 1;
        $_REQUEST['celid']  = $_REQUEST['celid']  ? $_REQUEST['celid']  : 2;
        $_REQUEST['funcao'] = $_REQUEST['funcao'] ? $_REQUEST['funcao'] : 238;
        
		if($_REQUEST["funcao"]) $andTec = " and ur.pflcod = ".$_REQUEST["funcao"];
        if($_REQUEST["celid"]){
			$andCel = " and ur.celid = ".$_REQUEST["celid"];
		}else{
			if($andCel){
				$andCel = $andCel;
			}
			else{
				$andCel = " and ur.celid = 2";	
			}
						
		}
		
		if($_REQUEST["ordid"] != '1' && $_REQUEST["ordid"] != '2' && $_REQUEST["ordid"] != '23'){
			$andCel = '';
			$andTec = '';
			$andOrd = " and ur.ordid = ".$_REQUEST["ordid"];
		}
		if($_REQUEST["ordid"] == '2' || $_REQUEST["ordid"] == '23'){
			$andTec = '';
		}

        $sql = "SELECT DISTINCT
                    u.usucpf,
                    u.usunome
                FROM
                    seguranca.usuario AS u
                INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
                WHERE
                    ur.rpustatus = 'A' AND
                    us.susstatus = 'A' AND
                    us.suscod = 'A'
                    $andTec
					$andCel
					$andOrd
                ORDER BY u.usunome";
		 //dbg($sql,1);
         $usuarios = $db->carregar( $sql );
        ?>
        <div class="row">
        	
        	<div align="center" style="cursor:pointer; vertical-align: bottom;  margin-top: 1px; color: #fff; font-weight: bold;" class="titulo_box" >
        		<?
        		if($_REQUEST["ordid"] == '1'){
        			echo strtoupper( ($_REQUEST['celid'] ? $db->pegaUm("SELECT celnome FROM demandas.celula WHERE celid = ".$_REQUEST['celid']) : '') );
        		}
        		else{
        			echo strtoupper( $db->pegaUm("SELECT orddescricao FROM demandas.origemdemanda WHERE ordid = ".$_REQUEST['ordid']) );
	        		if($_REQUEST["ordid"] == '2'){
	        			echo ' - '.strtoupper( ($_REQUEST['celid'] ? $db->pegaUm("SELECT celnome FROM demandas.celula WHERE celid = ".$_REQUEST['celid']) : '') );
	        		}
        		}
        		?>
        	</div>
        	
            <div class="col-md-12 box-principal">
                <div class="col-md-12">

                   	<div>
                   		<img style="float:left;" src="../imagens/icones/icons/obras.png">
                   	</div> 
                    <div   style="float:left;cursor:pointer; margin-top: 24px; color: #fff; font-weight: bold;"  >
                    	<?
                    	if($_REQUEST["ordid"] == '1'){
	                    	if($_REQUEST['funcao'] == '237') echo 'ANALISTAS';
	                    	else echo 'PROGRAMADORES';
                    	}else{
                    		echo 'TÉCNICOS';
                    	}
                    	?>
                    </div>


                    <div class="clearfix"></div>

                    <?php
					if($usuarios){
						
                    	foreach ($usuarios as $usuario) {

                    	//pega o analista com a demanda mais atrasada
                        $sql = "SELECT
                                    u.usunome as analista
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                LEFT JOIN
                                	seguranca.usuario u ON u.usucpf = d.usucpfanalise 
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND d.dmddatafimprevatendimento < CURRENT_DATE
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                order by d.dmddatafimprevatendimento
                                limit 1";
                        $analista = $db->PegaUm( $sql );
                        
                        //total demandas atrasadas
                        $sql = "SELECT
                                    count(*) as qtd
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND d.dmddatafimprevatendimento < CURRENT_DATE
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                ";
                        $atrasados = $db->PegaUm( $sql );

                        //total demandas que vencem hoje
                        $sql = "SELECT
                                    count(*) as qtd
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') = to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                ";
                        $emDia = $db->PegaUm( $sql );

                        //total demandas em dia
                        $sql = "SELECT
                                    count(*) as qtd
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') > to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                ";
                        $aVencer = $db->PegaUm( $sql );
                        

                        $class = 'zerado';

                        if ($atrasados) {
                            $class = 'atrasado';
                        } elseif($emDia) {
                            $class = 'em-dia';
                        } elseif($aVencer) {
                            $class = 'a-vencer';
                        }

                        ?>

                            <div class="col-md-2 box">
                                <div class="panel">
                                    <div class="panel-heading panel-<?php echo $class; ?> ver-detalhes" usucpf="<?php echo $usuario['usucpf']; ?>">
                                        <h3 class="panel-title">
                                            <?php
                                            echo substr($usuario['usunome'], 0, strpos($usuario['usunome'], ' ')) . substr($usuario['usunome'], strrpos($usuario['usunome'], ' '));
                                            ?>
                                        </h3>
                                    </div>
                                    <div class="panel-body panel-body-<?php echo $class; ?> ver-detalhes" usucpf="<?php echo $usuario['usucpf']; ?>" >
                                        <div style="width:90px; margin: 0 auto;">
                                            <span class="label label-danger"> <?php echo (int) $atrasados; ?></span>
                                            <span class="label label-warning"><?php echo (int) $emDia; ?></span>
                                            <span class="label label-success"><?php echo (int) $aVencer; ?></span>
                                        </div>
                                        <div style="margin-top: 14px;text-align: center;" align="center">
                                            <?if($analista){?>
                                            	<span class="label label" style="color: black; font-size: 11px;">
                                            		ANALISTA: 
                                            		<?
                                            			//echo substr($analista, 0, strpos($analista, ' ')) . substr($analista, strrpos($analista, ' ')); 
                                            			echo substr($analista, 0, strpos($analista, ' '));
                                            		?>
                                            	</span>
                                            <?}else{?>
                                            	<span class="label label" style="color: black;">&nbsp;</span>
                                            <?}?>
                                            </div>
                                        <div class="clearfix"></div>
                                    </div>
                                </div>
                            </div>

                    <?php }
					}
					else{
						echo '<center><font color="white"><b>Não existem registros.</b></font></center>';
					} ?>
                    <div class="clearfix"></div>
                </div>

                
            </div>
        </div>

		<!-- Modal -->
        <div class="modal fade" id="myModal" tabindex="-1" data-width="1000" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
            <div class="modal-header">
              <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
              <h4 class="modal-title">Entregas</h4>
            </div>
            <div class="modal-body" id="div-entregas"></div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
            </div>
        </div><!-- /.modal -->

        <!-- Modal -->
        <div class="modal fade" id="usuariosDetalhe" tabindex="-1" data-width="1200" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
            <div class="modal-header">
              <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
              <h4 class="modal-title">Usuários online por sistema</h4>
            </div>
            <div class="modal-body" id="div-usuarios"></div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
            </div>
        </div><!-- /.modal -->
        
       
    </body>
</html>

<?

/*
 * Funções
 */

/**
 * Função utilizada para montar o painel de monitoramento
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[ano] Ano do monitoramento
 * @param integer $dados[mes] Mês do monitoramento 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 11/11/2009
 */
function monitoramentoGRID($dados) {
	$db = new cls_banco();
	
	// se for filtro por perído (aplicar regras) 
	if($dados['diaini'] && $dados['diafim']) {
		
		$sql = "SELECT monsisdsc, CASE WHEN m.monsisdiretorio IS NOT NULL THEN m.monsisdiretorio ELSE m.sisid::text END as sisid, m.tmoid, m.monvalor::numeric as monvalor, tp.tmoacao FROM seguranca.monitoramento m 
				LEFT JOIN seguranca.tipomonitoramento tp ON m.tmoid=tp.tmoid  
				WHERE monano='".$dados['ano']."' AND monmes='".$dados['mes']."' AND mondia>=".$dados['diaini']." AND mondia<=".$dados['diafim'];
		$respostas = $db->carregar($sql);
		
		if($respostas[0]) {
			foreach($respostas as $rsp) {
				$_ACAO[$rsp['tmoid']] = trim($rsp['tmoacao']);
				
				if(!is_numeric($rsp['sisid'])) {
					$_SISTEMAS_EX[$rsp['sisid']] = $rsp['monsisdsc'];
				}
				
				$_GRID[$rsp['sisid']][$rsp['tmoid']]+= $rsp['monvalor'];
			}
			
			foreach($_GRID as $sisid => $da) {
				foreach($da as $tmoid => $valor) {
					switch($_ACAO[$tmoid]) {
						case 'media':
							$_GRID[$sisid][$tmoid] = round($valor/((integer)$dados['diafim']-(integer)$dados['diaini']+1), 4);
							break;
					}
				}
			}
		}
	} else {
		$sql = "SELECT monsisdsc, CASE WHEN monsisdiretorio IS NOT NULL THEN monsisdiretorio ELSE sisid::text END as sisid, tmoid, monvalor::numeric as monvalor FROM seguranca.monitoramento WHERE monano='".$dados['ano']."' AND monmes='".$dados['mes']."' AND mondia IS NULL";
		$respostas = $db->carregar($sql);
		
		if($respostas[0]) {
			foreach($respostas as $rsp) {

				if(!is_numeric($rsp['sisid'])) {
					$_SISTEMAS_EX[$rsp['sisid']] = $rsp['monsisdsc'];
				}

				$_GRID[$rsp['sisid']][$rsp['tmoid']]= $rsp['monvalor'];
			}
		}
	}
	
	$_HTML .= "<table class='label-tabela' width='100%' cellspacing='3' cellpadding='3'	align='center'>";

	$sql = "SELECT sisid, sisdsc, u.usunome 
			FROM seguranca.sistema s
			LEFT JOIN seguranca.usuario u ON u.usucpf = s.usucpfanalista 
			WHERE sisstatus='A' AND sisid!=4";
	$sistemas = $db->carregar($sql);
	
	if($_SISTEMAS_EX) {
		foreach($_SISTEMAS_EX as $monsisdiretorio => $monsisdsc) {
			$sistemas[] = array('sisid' => $monsisdiretorio, 'sisdsc' => $monsisdsc, 'usunome' => 'WALLACE CARDOSO PEREIRA');
		}
	}

	$sql = "SELECT * FROM seguranca.tipomonitoramento WHERE tmoativo='A' and tmoid in (".$dados['tmoids'].") ORDER BY tmoordem";
	$tipomonitoramento = $db->carregar($sql);

	if($sistemas[0]) {
		
		$_HTML .= "<tr style='background: #2554C7;'>";
		$_HTML .= "<td align=center style='font-weight: bold;font-size:x-small;' align='center'>RANKING</td>";
		$_HTML .= "<td align=center style='font-weight: bold;font-size:x-small;'>RESPONSÁVEIS</td>";
		$_HTML .= "<td align=center style='font-weight: bold;font-size:x-small;'>MÓDULOS</td>";
		if($tipomonitoramento[0]) {
			foreach($tipomonitoramento as $tpm) {
				$sigla = $tpm['tmosiglatipo'];
				if($sigla == 'NE') $sigla = "ERROS";
				if($sigla == 'NR') $sigla = "REQUISIÇÕES";
				if($sigla == 'PE') $sigla = "PERCENTUAIS";
				if($sigla == 'TM') $sigla = "TEMPO MÉDIO DE EXECUÇÃO";
				$_HTML .= "<td style='font-weight: bold;font-size:x-small;' align='center' title=\"".$tpm['tmodescricao']."\">".$sigla."</td>";
			}
		} else {
			$_HTML .= "<td>Não existem tipos de monitoramento</td>";
		}
		$_HTML .= "</tr>";
		
		unset($HTML);
		
		foreach($sistemas as $sis) {
			$HTML[$sis['sisid']] .= "<tr>";
			$HTML[$sis['sisid']] .= "<td align='center' style=font-size:x-small;>{rankingplace}</td>";
			$HTML[$sis['sisid']] .= "<td nowrap style=font-size:x-small;>".($sis['usunome']?$sis['usunome']:'-')."</td>";
			$HTML[$sis['sisid']] .= "<td style=font-size:x-small;>".$sis['sisdsc']."</td>";
			if($tipomonitoramento[0]) {
				foreach($tipomonitoramento as $tpm) {
					unset($vls,$args,$style,$cor1,$cor2);
					$style="style=\"font-size:x-small\"";
					if($tpm['tmoparametros']) {
						$args = explode(";",$tpm['tmoparametros']);
						if($args[0]) {
							$vls = explode(":",$args[0]);
							
							$cor1 = $vls[1];
							$cor2 = $vls[2];
							
							if($cor1=='#FFFF66') $cor1='orange'; 
							if($cor2=='#FFFF66') $cor2='orange';
							
							if($cor1=='#66FF99') $cor1='green'; 
							if($cor2=='#66FF99') $cor2='green';
							
							if($cor1=='#FF3333') $cor1='#f00'; 
							if($cor2=='#FF3333') $cor2='#f00';
							$cor1 = '';
							$cor2 = '';
							
							if($_GRID[$sis['sisid']][$tpm['tmoid']] <= $vls[0]) $style="style=\"font-size:x-small;background-color:".$cor1.";\" title=\"{$cor2}\"";
						}
						if($args[1]) {
							$vls = explode(":",$args[1]);
							
							$cor1 = $vls[1];
							$cor2 = $vls[2];
							
							if($cor1=='#FFFF66') $cor1='orange'; 
							if($cor2=='#FFFF66') $cor2='orange';
							
							if($cor1=='#66FF99') $cor1='green'; 
							if($cor2=='#66FF99') $cor2='green';
							
							if($cor1=='#FF3333') $cor1='#f00'; 
							if($cor2=='#FF3333') $cor2='#f00';
							
							$cor1 = '';
							$cor2 = '';
							
							if($_GRID[$sis['sisid']][$tpm['tmoid']] >= $vls[0]) $style="style=\"font-size:x-small;background-color:".$cor1.";\" title=\"{$cor2}\"";
						}
					}
					$_ORDEM[$tpm['tmoid']][$sis['sisid']] = $_GRID[$sis['sisid']][$tpm['tmoid']];
					$_TOTAL[$tpm['tmoid']][] = array('valor' => $_GRID[$sis['sisid']][$tpm['tmoid']], 'acao' => $tpm['tmoacao']);
					$HTML[$sis['sisid']] .= "<td align='center' {$style}>".(($_GRID[$sis['sisid']][$tpm['tmoid']])?"<b>".$_GRID[$sis['sisid']][$tpm['tmoid']]."</b>":"0").($tpm['tmoid']==PE?' %':'')."</td>";		
				}
			} else {
				$HTML[$sis['sisid']] .= "<td>&nbsp;</td>";
			}
			$HTML[$sis['sisid']] .= "</tr>";
		}
		
		//ordena crescente
		//asort($_ORDEM[PE]);
		
		//ordena decrescente
		arsort($_ORDEM[$dados['ordem']]);
		arsort($_TOTAL[$dados['ordem']]);
		
		if($dados['ordem']==NE) {
			foreach($_ORDEM[NE] as $sisid => $indice) {
				$_ORDEM['MERGE'][(($indice)?$indice:"N")][$_ORDEM[NR][$sisid]] = $sisid;
			}
		
		} elseif($dados['ordem']==TM) {

			foreach($_ORDEM[TM] as $sisid => $indice) {
				$_ORDEM['MERGE'][(($indice)?$indice:"N")][$_ORDEM[NR][$sisid]] = $sisid;
			}
			
		}
		
		$_ORDEM['FINAL'] = array();
		foreach($_ORDEM['MERGE'] as $ar) {
			krsort($ar);
			foreach($ar as $si) {
				$_ORDEM['FINAL'][] = $si;
			}
		}
		
		$rank=1; //controla a quantidade na listagem
		foreach($_ORDEM['FINAL'] as $sisid) {
			if($rank<11){ //controla a quantidade na listagem
				$_HTML .= str_replace("{rankingplace}", $rank."º", $HTML[$sisid]);
			}
			$rank++; 
		}
		
	}
	$_HTML .= "</table>"; 
	
	echo $_HTML;
}


/**
 * Função utilizada para carregar as informações
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[ano] Ano do monitoramento
 * @param integer $dados[mes] Mês do monitoramento
 * @param integer $dados[tmoid] Tipo do monitoramento
 * @param integer $dados[sisid] ID do sistema 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 11/11/2009
 */
function pegarDados($dados) {
	$db = new cls_banco();
	
	$sql = "SELECT * FROM seguranca.tipomonitoramento WHERE tmoativo='A' ORDER BY tmoordem";
	$dadostp = $db->carregar($sql);
	
	if($dadostp[0]) {
		foreach($dadostp as $tp) {
			$dadosc[$tp['tmoid']] = array();
			
			$sql = "SELECT m.tmoid, m.mondia, m.monvalor::numeric as monvalor FROM seguranca.monitoramento m 
					LEFT JOIN seguranca.tipomonitoramento tm ON tm.tmoid=m.tmoid
					WHERE m.sisid='".$dados['sisid']."' AND tm.tmoid='".$tp['tmoid']."' AND m.monano='".$dados['ano']."' AND m.monmes='".$dados['mes']."' AND m.mondia IS NOT NULL";
			
			$dadosfn = $db->carregar($sql);
			if($dadosfn[0]) {
				foreach($dadosfn as $d) {
					$dadosc[$tp['tmoid']][$d['mondia']] = $d['monvalor'];
				}
			}
		}
	}
	
	return $dadosc;
}

function pegarDiasMes($dados) {
	echo cal_days_in_month(CAL_GREGORIAN, $dados['mes'], $dados['ano']);	
}


function pegarDadosPorPagina($dados) {
	$db = new cls_banco();
	
	$resultado = $db->carregar("SELECT COUNT(e.oid) as num,  to_char(estdata, 'DD') as dia  FROM seguranca.estatistica e 
								INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid 
								WHERE e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[NR] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT COUNT(DISTINCT u.usucpf) as num,  to_char(estdata, 'DD') as dia FROM seguranca.estatistica e
								INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid 
								LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf 
								WHERE e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[NU] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT COUNT(au.oid) as num, to_char(auddata, 'DD') as dia FROM seguranca.auditoria au 
							    INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
							    LEFT JOIN seguranca.usuario u ON u.usucpf=au.usucpf 
								WHERE me.sisid='".$dados['sisid']."' AND au.audtipo='X' AND (date_part('year',auddata)::varchar||date_part('month',auddata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND me.mnulink ILIKE '%".$dados['link']."%'
								GROUP BY to_char(auddata, 'DD') ORDER BY to_char(auddata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[NE] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT ROUND(CAST(AVG(estmemusa) as numeric),2) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica e 
						 		INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid
								WHERE e.estmemusa IS NOT NULL AND e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[MU] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT ROUND(CAST(AVG(esttempoexec) as numeric),2) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica e 
						 		INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid
								WHERE e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[TM] = $result;
		unset($result); 
	}
	
	return $resul;
	
}

function dscMes($mes){
	
	$mes = (int) $mes;
	
	switch ($mes) {
    case 1:
        echo "JANEIRO";
        break;
    case 2:
        echo "FEVEREIRO";
        break;
    case 3:
        echo "MARÇO";
        break;
    case 4:
        echo "ABRIL";
        break;
    case 5:
        echo "MAIO";
        break;
    case 6:
        echo "JUNHO";
        break;
    case 7:
        echo "JULHO";
        break;
    case 8:
        echo "AGOSTO";
        break;
    case 9:
        echo "SETEMBRO";
        break;
    case 10:
        echo "OUTUBRO";
        break;
    case 11:
        echo "NOVEMBRO";
        break;
    case 12:
        echo "DEZEMBRO";
        break;
	}    
}

function montarGrafico()
{ ?>

    <script>
        $(function () {
            $('#container').highcharts({

                chart: {
                    type: 'gauge',
                    plotBackgroundColor: null,
                    plotBackgroundImage: null,
                    plotBorderWidth: 0,
                    plotShadow: false,
                    backgroundColor:'rgba(255, 255, 255, 0.0)'
                },

                title: {
                    text: ''
                },

                //habilitar o botão de salvar como imagem, pdf, etc
                exporting: {
                    enabled: false
                },
                credits: {
                    enabled: false
                },

                pane: {
                    startAngle: -150,
                    endAngle: 150,
                    background: [{
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#FFF'],
                                [1, '#000']
                            ]
                        },
                        borderWidth: 0,
                        outerRadius: '109%'
                    }, {
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#333'],
                                [1, '#FFF']
                            ]
                        },
                        borderWidth: 1,
                        outerRadius: '107%'
                    }, {
                        // default background
                    }, {
                        backgroundColor: '#DDD',
                        borderWidth: 0,
                        outerRadius: '105%',
                        innerRadius: '103%'
                    }]
                },

                // the value axis
                yAxis: {
                    min: 0,
                    max: 100,

                    minorTickInterval: 'auto',
                    minorTickWidth: 1,
                    minorTickLength: 10,
                    minorTickPosition: 'inside',
                    minorTickColor: '#666',

                    tickPixelInterval: 30,
                    tickWidth: 2,
                    tickPosition: 'inside',
                    tickLength: 10,
                    tickColor: '#666',
                    labels: {
                        step: 2,
                        rotation: 'auto'
                    },
                    title: {
                        text: 'Qtd.'
                    },
                    plotBands: [{
                        from: 0,
                        to: 20,
                        color: '#55BF3B' // green
                    }, {
                        from: 20,
                        to: 50,
                        color: '#DDDF0D' // yellow
                    }, {
                        from: 50,
                        to: 100,
                        color: '#DF5353' // red
                    }]
                },
                series: [{
                    name: 'Queries',
                    data: [parseInt(localStorage.queryQtd)],

                }]

            },
            // Add some life
            function (chart) {
                if (!chart.renderer.forExport) {
                    setInterval(function () {
                        $.ajax({
                            url: '/demandas/popPainelGerencia.php?atualizar_query=1',
                            success: function(resultado){
                                resultado = isNaN(parseInt(resultado)) ? 0 : resultado;
                                var point = chart.series[0].points[0];
                                point.update(parseInt(resultado));
                                localStorage.queryQtd = parseInt(resultado);
                            }
                        });

                    }, 1000);
                }
            });

            $('#container_tempo').highcharts({

                chart: {
                    type: 'gauge',
                    plotBackgroundColor: null,
                    plotBackgroundImage: null,
                    plotBorderWidth: 0,
                    plotShadow: false,
                    backgroundColor:'rgba(255, 255, 255, 0.0)'
                },

                title: {
                    text: ''
                },

                //habilitar o botão de salvar como imagem, pdf, etc
                exporting: {
                    enabled: false
                },
                credits: {
                    enabled: false
                },

                pane: {
                    startAngle: -150,
                    endAngle: 150,
                    background: [{
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#FFF'],
                                [1, '#000']
                            ]
                        },
                        borderWidth: 0,
                        outerRadius: '109%'
                    }, {
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#333'],
                                [1, '#FFF']
                            ]
                        },
                        borderWidth: 1,
                        outerRadius: '107%'
                    }, {
                        // default background
                    }, {
                        backgroundColor: '#DDD',
                        borderWidth: 0,
                        outerRadius: '105%',
                        innerRadius: '103%'
                    }]
                },

                // the value axis
                yAxis: {
                    min: 0,
                    max: 1000,

                    minorTickInterval: 'auto',
                    minorTickWidth: 1,
                    minorTickLength: 10,
                    minorTickPosition: 'inside',
                    minorTickColor: '#666',

                    tickPixelInterval: 30,
                    tickWidth: 2,
                    tickPosition: 'inside',
                    tickLength: 10,
                    tickColor: '#666',
                    labels: {
                        step: 2,
                        rotation: 'auto'
                    },
                    title: {
                        text: 'Tempo'
                    },
                    plotBands: [{
                        from: 0,
                        to: 100,
                        color: '#55BF3B' // green
                    }, {
                        from: 100,
                        to: 300,
                        color: '#DDDF0D' // yellow
                    }, {
                        from: 300,
                        to: 1000,
                        color: '#DF5353' // red
                    }]
                },
                series: [{
                    name: 'Queries',
                    data: [parseInt(localStorage.queryTempo)],

                }]

            },
            // Add some life
            function (chart) {
                if (!chart.renderer.forExport) {
                    setInterval(function () {
                        $.ajax({
                            url: '/demandas/popPainelGerencia.php?atualizar_query_tempo=1',
                            success: function(resultado){
                                resultado = isNaN(parseInt(resultado)) ? 0 : resultado;
                                var point = chart.series[0].points[0];
                                point.update(parseInt(resultado));
                                localStorage.queryTempo = parseInt(resultado);
                            }
                        });

                    }, 1000);
                }
            });

            $('#container_pdeinterativo').highcharts({

                chart: {
                    type: 'gauge',
                    plotBackgroundColor: null,
                    plotBackgroundImage: null,
                    plotBorderWidth: 0,
                    plotShadow: false,
                    backgroundColor:'rgba(255, 255, 255, 0.0)'
                },

                title: {
                    text: ''
                },

                //habilitar o botão de salvar como imagem, pdf, etc
                exporting: {
                    enabled: false
                },
                credits: {
                    enabled: false
                },

                pane: {
                    startAngle: -150,
                    endAngle: 150,
                    background: [{
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#FFF'],
                                [1, '#000']
                            ]
                        },
                        borderWidth: 0,
                        outerRadius: '109%'
                    }, {
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#333'],
                                [1, '#FFF']
                            ]
                        },
                        borderWidth: 1,
                        outerRadius: '107%'
                    }, {
                        // default background
                    }, {
                        backgroundColor: '#DDD',
                        borderWidth: 0,
                        outerRadius: '105%',
                        innerRadius: '103%'
                    }]
                },

                // the value axis
                yAxis: {
                    min: 0,
                    max: 100,

                    minorTickInterval: 'auto',
                    minorTickWidth: 1,
                    minorTickLength: 10,
                    minorTickPosition: 'inside',
                    minorTickColor: '#666',

                    tickPixelInterval: 30,
                    tickWidth: 2,
                    tickPosition: 'inside',
                    tickLength: 10,
                    tickColor: '#666',
                    labels: {
                        step: 2,
                        rotation: 'auto'
                    },
                    title: {
                        text: 'Qtd.'
                    },
                    plotBands: [{
                        from: 0,
                        to: 20,
                        color: '#55BF3B' // green
                    }, {
                        from: 20,
                        to: 50,
                        color: '#DDDF0D' // yellow
                    }, {
                        from: 50,
                        to: 100,
                        color: '#DF5353' // red
                    }]
                },
                series: [{
                    name: 'Queries',
                    data: [parseInt(localStorage.queryQtd)],

                }]

            },
            // Add some life
            function (chart) {
                if (!chart.renderer.forExport) {
                    setInterval(function () {
                        $.ajax({
                            url: '/demandas/popPainelGerencia.php?atualizar_pdeinterativo_query=1',
                            success: function(resultado){
                                resultado = isNaN(parseInt(resultado)) ? 0 : resultado;
                                var point = chart.series[0].points[0];
                                point.update(parseInt(resultado));
                                localStorage.queryQtd = parseInt(resultado);
                            }
                        });

                    }, 1000);
                }
            });

            $('#container_pdeinterativo_tempo').highcharts({

                chart: {
                    type: 'gauge',
                    plotBackgroundColor: null,
                    plotBackgroundImage: null,
                    plotBorderWidth: 0,
                    plotShadow: false,
                    backgroundColor:'rgba(255, 255, 255, 0.0)'
                },

                title: {
                    text: ''
                },

                //habilitar o botão de salvar como imagem, pdf, etc
                exporting: {
                    enabled: false
                },
                credits: {
                    enabled: false
                },

                pane: {
                    startAngle: -150,
                    endAngle: 150,
                    background: [{
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#FFF'],
                                [1, '#000']
                            ]
                        },
                        borderWidth: 0,
                        outerRadius: '109%'
                    }, {
                        backgroundColor: {
                            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                            stops: [
                                [0, '#333'],
                                [1, '#FFF']
                            ]
                        },
                        borderWidth: 1,
                        outerRadius: '107%'
                    }, {
                        // default background
                    }, {
                        backgroundColor: '#DDD',
                        borderWidth: 0,
                        outerRadius: '105%',
                        innerRadius: '103%'
                    }]
                },

                // the value axis
                yAxis: {
                    min: 0,
                    max: 1000,

                    minorTickInterval: 'auto',
                    minorTickWidth: 1,
                    minorTickLength: 10,
                    minorTickPosition: 'inside',
                    minorTickColor: '#666',

                    tickPixelInterval: 30,
                    tickWidth: 2,
                    tickPosition: 'inside',
                    tickLength: 10,
                    tickColor: '#666',
                    labels: {
                        step: 2,
                        rotation: 'auto'
                    },
                    title: {
                        text: 'Tempo'
                    },
                    plotBands: [{
                        from: 0,
                        to: 100,
                        color: '#55BF3B' // green
                    }, {
                        from: 100,
                        to: 300,
                        color: '#DDDF0D' // yellow
                    }, {
                        from: 300,
                        to: 1000,
                        color: '#DF5353' // red
                    }]
                },
                series: [{
                    name: 'Queries',
                    data: [parseInt(localStorage.queryTempo)],

                }]

            },
            // Add some life
            function (chart) {
                if (!chart.renderer.forExport) {
                    setInterval(function () {
                        $.ajax({
                            url: '/demandas/popPainelGerencia.php?atualizar_pdeinterativo_query_tempo=1',
                            success: function(resultado){
                                resultado = isNaN(parseInt(resultado)) ? 0 : resultado;
                                var point = chart.series[0].points[0];
                                point.update(parseInt(resultado));
                                localStorage.queryTempo = parseInt(resultado);
                            }
                        });

                    }, 1000);
                }
            });

            setInterval(function () {
                $('#usuarios_online').load('/demandas/popPainelGerencia.php?useronline=1');
                $('#usuarios_online_pdeinterativo').load('/demandas/popPainelGerencia.php?useronline_pdeinterativo=1');
            }, 3000);

            $('tspan').each(function(i, obj){
                if($(obj).html() == 'Tempo' || $(obj).html() == 'Qtd.'){
                    $(obj).attr('y', '75')
                }
            });
        });
    </script>

<?php } ?>
