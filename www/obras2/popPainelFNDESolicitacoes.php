<?php
/**
 * Setor responsvel: SIEMC
 * Autor: Victor Martins Machado <VictorMachado@mec.org.br>
 * Módulo: Obras2
 * Finalidade: Representa as restrições e inconformidades
 * Data de criação: 25/11/2014
 * Última modificação:
 */


// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_constantes.php";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//ver($_POST['tpoid']);
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
        
        <script src="/library/chosen-1.0.0/chosen.jquery.js"></script>
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="/library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>
        <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
                
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
        <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
		<script type="text/javascript" src="../includes/superTitle.js"></script>
        
        <script src="/library/multiple-select/jquery.multiple.select.js" type="text/javascript"></script>
        <link href="/library/multiple-select/multiple-select.css" rel="stylesheet"/>
				
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
                height: 130px;
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
            .label-pausada {background: #949494;}
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
            tr.pausada th, tr.pausada td {background: #E8E8E8 !important;}

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
        		    $('.chosen').chosen({no_results_text: "Nenhum registro encontrado: "});
        		$('.ver-detalhes').click(function(){
            		a = window.open('popupSolicitacoesPainel.php?rstitem='+$('#esdid').val()+'&tpoid='+$('#tpoid').val()+'&estuf='+$(this).attr('estuf')+'&filtro='+$(this).attr('situacao'), 'Combopopup', "height=500,width=1500,scrollbars=yes,top=50,left=150" );
            		a.focus();
        		});

        		$('.label-danger').click(function(){
            		a = window.open('popupSolicitacoesPainel.php?rstitem='+$('#esdid').val()+'&tpoid='+$('#tpoid').val()+'&estuf='+$(this).attr('estuf')+'&filtro='+$(this).attr('situacao'), 'Combopopup', "height=500,width=1400,scrollbars=yes,top=50,left=150" );
            		a.focus();
        		});

        		$('.label-warning').click(function(){
            		a = window.open('popupSolicitacoesPainel.php?rstitem='+$('#esdid').val()+'&tpoid='+$('#tpoid').val()+'&estuf='+$(this).attr('estuf')+'&filtro='+$(this).attr('situacao'), 'Combopopup', "height=500,width=1400,scrollbars=yes,top=50,left=150" );
            		a.focus();
        		});

        		$('.label-success').click(function(){
            		a = window.open('popupSolicitacoesPainel.php?rstitem='+$('#esdid').val()+'&tpoid='+$('#tpoid').val()+'&estuf='+$(this).attr('estuf')+'&filtro='+$(this).attr('situacao'), 'Combopopup', "height=500,width=1400,scrollbars=yes,top=50,left=150" );
            		a.focus();
        		});
        		
        	});

        	function pegaItem(val) {
    			$('#rstitem').val(val);
    		}

    		function pegaTipo(val) {
    			$('#tprid').val(val);
    			$('#formPainel').submit();
    		}

    		function pegaTipologia() {
				$('#formPainel').submit();
    		}
        </script>

    </head>

    <body>
        <?php
        $sql = "SELECT
 					est.estuf
 				FROM territorios.estado est
 				ORDER BY est.estuf";

        $estados = $db->carregar( $sql );
        ?>
        <div class="row">
        	
        	<div align="center" style="cursor:pointer; vertical-align: bottom;  margin-top: 1px; color: #fff; font-weight: bold;" class="titulo_box" >
        		PAINEL DE SOLICITAÇÕES
        	</div>
        	
            <div class="col-md-12 box-principal">
                <div class="col-md-12">
					<form id="formPainel" name="formPainel" method="POST">
	                    <div class="col-md-12"  style="float:left; margin-top: 15px; color: #fff; font-weight: bold;">
	                    	<div class="col-md-3">
	                    		<div class="row">
		                    		<span>Situação:</span>
					                <?php
					                $val = (!empty($_POST['esdid'])) ? $_POST['esdid'] : '';

                                    $sql = " SELECT esdid as codigo, esddsc as descricao
					                         FROM workflow.estadodocumento
					                         WHERE esdid IN (" . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA . ")";
					                $db->monta_combo("esdid", $sql, "S", "Todos", "", "", "", "200", "N", "esdid", false, $val, '', '', 'chosen');
					                ?>
				                </div>
                    		</div>
	                    	
	                    	<div class="col-md-4">
	                    		<div class="row">
		                    		<span>Tipologia da Obra:</span>
	                                <?php
	                                $val = (!empty($_POST['tpoid'])) ? $_POST['tpoid'] : '';
	                                
	                                $sql = "SELECT tpoid AS codigo, tpodsc AS descricao FROM obras2.tipologiaobra WHERE tpostatus = 'A' AND orgid = 3 ORDER BY tpodsc";
	                                $db->monta_combo_multiplo("tpoid", $sql, "S",'','','', '', 1, '300',array(), "tpoid", false, 'multiple');
	                                ?>
	                                <script type="text/javascript">
		                				var t = '<?php echo !empty($_POST['tpoid']) ? implode(",", $_POST['tpoid']) : ''; ?>';
		                				var a = t.split(",");
		                				
										
		                				$('#tpoid').multipleSelect({ 
			                			    filter: true,
			                			    placeholder: 'Todos',
			                			    selectAllText: 'Selecione todos',
			                			    allSelected: 'Todos selecionados',
			                			    minimumCountSelected: 2,
			                			    countSelected: 'Selecionados, # de %'
		                			    });

		                				//$('#tpoid').setSelects(a);
		                				$('#tpoid').multipleSelect('setSelects', a);
	                                </script>
                                </div>
	                    	</div>
	                    	
	                    	<div class="col-md-2">
	                    		<div class="row">
	                    			<input type="submit" name="pesquisar" class="pesquisar" value="Pesquisar"/>
	                    		</div>
	                    	</div>
	                    </div>
	                    <div class="clearfix"></div>
						<br />
						
	                    <?php 
	                    
	                    $vermelho_b = 0;
	                    $amarelo_b = 0;
	                    $verde_b = 0;
	                    $html = "";
	                    
	                    if ($_POST['esdid']){
							$item = " AND d.esdid = '{$_POST['esdid']}' ";
						}

						
						if ($_POST['tpoid'] && $_POST['tpoid'] != '') {
							$tipologia = " AND o.tpoid in (".implode(",", $_POST['tpoid']).")";
						}


                        $sql = "
                            SELECT
                                et.estuf,
                                COUNT(p.vermelho) vermelho,
                                COUNT(p.amarelo) amarelo,
                                COUNT(p.verde) verde
                                FROM territorios.estado et
                                LEFT JOIN(
                                SELECT
                                    mun.estuf,
                                    CASE WHEN DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao) ) < 5 THEN '1' END as verde,
                                    CASE WHEN (DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao)) >= 5 AND DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao)) <= 10) THEN '2' END as amarelo,
                                    CASE WHEN DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao)) > 10 THEN '3' END as vermelho
                                FROM obras2.solicitacao_desembolso s
                                JOIN obras2.obras o ON o.obrid = s.obrid AND o.obridpai IS NULL  AND o.obrstatus = 'A'
                                JOIN workflow.documento d ON d.docid = s.docid AND d.esdid IN (1575, 1581, 1597)
                                JOIN entidade.endereco ende ON ende.endid = o.endid
                                JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                                LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid AND h.aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = d.esdid AND aedstatus = 'A')
                                RIGHT JOIN territorios.estado est ON est.estuf = mun.estuf
                                WHERE
                                    s.sldstatus = 'A'
                                    $tipologia
                                    $item
                                ) as p ON p.estuf = et.estuf
                                GROUP BY et.estuf
                                ORDER BY et.estuf


                        ";

                        $estados = $db->carregar($sql);

						//ver($estados);
	                    foreach ($estados as $estado) {

	                        $verde = $estado['verde'];
	                        $verde_b += $verde;
	                        //$analista = 0;

                            $amarelo = $estado['amarelo'];
	                        $amarelo_b += $estado['amarelo'];
	                        //$atrasados = 0;

                            $vermelho = $estado['vermelho'];
	                        $vermelho_b += $estado['vermelho'];
	                        //$emDia = 0;

	                        //$class = 'zerado';
	                        $class = 'a-vencer';
							
	                        if ($estado['vermelho']) {
	                            $class = 'atrasado';
	                        } elseif($estado['verde']) {
								$class = 'a-vencer';
	                        } elseif($estado['amarelo']) {
								$class = 'em-dia';
	                        }
							
	                        
	                        $html .= "
	                            <div class=\"col-md-2 box\">
	                                <div class=\"panel panel-body-".$class."\">
	                                    <div class=\"panel-heading panel-".$class." ver-detalhes\" estuf=\"".$estado['estuf']."\" situacao=\"\">
	                                        <h3 class=\"panel-title\">
	                                            ".$estado['estuf']."
	                                        </h3>
	                                    </div>
	                                    <div class=\"panel-body panel-body-".$class."\" estuf=\"".$estado['estuf']."\">
	                                        <div style=\"width:100%; margin-left: 17px\">
	                                            <span class=\"label label-danger\" estuf=\"".$estado['estuf']."\" situacao=\"R\"> ".(int) $vermelho."</span>
	                                            <span class=\"label label-warning\" estuf=\"".$estado['estuf']."\" situacao=\"A\"> ". (int) $amarelo."</span>
	                                            <span class=\"label label-success\" estuf=\"".$estado['estuf']."\" situacao=\"V\"> ".(int) $verde."</span>
	                                        </div>
	                                        <div style=\"margin-top: 14px;text-align: center;\" align=\"center\">";
                            $html .= "<span class=\"label label\" style=\"color: black; font-size: 11px;\">";
                            $sql = "SELECT
                                        usu.usunome
                                    FROM obras2.usuariofnderestricao ufr
                                    INNER JOIN seguranca.usuario usu
                                        ON (ufr.usucpf = usu.usucpf)
                                    WHERE ufr.estuf = '{$estado['estuf']}'
                                    {$tipoResp}";

                            $usuarios = $db->carregar($sql);
                            $usuarios = is_array($usuarios) ? $usuarios : array();

                            foreach ($usuarios as $usu) {
                                $html .= $usu['usunome'] . '<br/>';
                            }

                            $html .= "</span>";
                            $html .= "
	                                        </div>
	                                        <div class=\"clearfix\"></div>
	                                    </div>
	                                </div>
	                            </div>";
                        }

                        $htmlB = '';
                        //$class = 'zerado';
                        $class = 'a-vencer';

                        if ($vermelho_b) {
                            $class = 'atrasado';
                        } elseif($verde_b) {
                            $class = 'a-vencer';
                        } elseif($amarelo_b) {
                            $class = 'em-dia';
                        }

                        $htmlB .= "
                            <div class=\"col-md-2 box\">
                                <div class=\"panel panel-body-".$class."\">
                                    <div class=\"panel-heading panel-".$class." ver-detalhes\" estuf=\"\" situacao=\"\">
                                        <h3 class=\"panel-title\">
                                            BRASIL - TOTAL
                                        </h3>
                                    </div>
                                    <div class=\"panel-body panel-body-".$class."\" estuf=\"".$estado['estuf']."\">
                                        <div style=\"width:100%; margin-left: 17px\">
                                            <span class=\"label label-danger\" estuf=\"\" situacao=\"R\"> ".(int) $vermelho_b."</span>
                                            <span class=\"label label-warning\" estuf=\"\" situacao=\"A\"> ". (int) $amarelo_b."</span>
                                            <span class=\"label label-success\" estuf=\"\" situacao=\"V\"> ".(int) $verde_b."</span>
                                        </div>
                                        <div style=\"margin-top: 14px;text-align: center;\" align=\"center\">
                                        </div>
                                        <div class=\"clearfix\"></div>
                                    </div>
                                </div>
                            </div>";
                        echo $htmlB.$html;
                        ?>
					
	                    <div class="clearfix"></div>
                    </form>
                </div>
            </div>
        </div>
    </body>
</html>
