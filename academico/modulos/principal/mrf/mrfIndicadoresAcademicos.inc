<?php
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']( $_REQUEST );
        die();
    }
    
    #CASO A SESSAO "['academico']['orgid']" POR ALGUM MOTIVO FAZ O TRATAMENTO ABAIXO, PARA QUE $funid NÃO ASSUMA VALOR NULO.
    if(empty($_SESSION['academico']['orgid'])){
        $db->insucesso('Não é possivél visualizar os Indicadores Acadêmicos, tente novamente mais tarde!', '', 'inicio&acao=C');
        die();
    }
    
    function buscaDadosGrafico(){
        global $db;
        
        $sql = "SELECT  	CASE 
                	       		WHEN trim(ida.idaigc) = '3' THEN 'IGC 3'
                       			WHEN trim(ida.idaigc) = '4' THEN 'IGC 4'
                       			WHEN trim(ida.idaigc) = '5' THEN 'IGC 5'
                       			ELSE 'Sem Conceito'
                    		END AS igc_descricao,
                    		COUNT(ida.entid) AS total_conceito	
            	FROM	 	academico.indicadoresacademicos ida
	            GROUP BY 	igc_descricao
	            ORDER BY 	igc_descricao";
        
        $dados = $db->carregar($sql);
        foreach ($dados as $i => $item){
            if($i == 0){
                $array_valores .= "{ name: '".$item['igc_descricao']."', y: ".$item['total_conceito'].", sliced: true, selected: true },";
            } else {
                $array_valores .= '[\''.$item['igc_descricao'].'\','.$item['total_conceito'].']';
            }
        }
        $array_valores = str_replace('][', '],[', $array_valores);
        return  $array_valores;
        die();
    }
    
    include APPRAIZ . 'includes/cabecalho.inc';

    monta_titulo('Indicadores Acadêmicos', '');
    echo "<br>"
?>	

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<script>
    $(document).ready(function() {
        $('.documentos').click(function() {
            var id = $(this).attr('id');
            var imiid = $(this).attr('imiid');
            var janela = window.open('academico.php?modulo=principal/mrf/mrfIndicadoresAcademicos&acao=A&popup=popupMostraDocumentosMRF&idaid=' + id + '&imiid=' + imiid, 'altera_documento', 'width=1200,height=800,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        });
        $('.demanda').click(function() {
            var id = $(this).attr('id');
            var janela = window.open('academico.php?modulo=principal/mpa/popupMostraDemandaMPAInd&acao=A&idaid=' + id + '&view=S', '', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        });
    });
    
    $(function () {
        
        <?php $array_valores = buscaDadosGrafico(); ?>
      
        //Radialize the colors
        Highcharts.getOptions().colors = Highcharts.map(Highcharts.getOptions().colors, function(color) {
            return {
                radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                stops: [
                    [0, color],
                    [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                ]
            };
        });
        
        //Build the chart
        $('#container').highcharts({ 
            lang: {
                printChart: 'Imprimir',
                downloadPDF: 'Exportar em PDF',
                downloadJPEG: 'Exportar em JPG',
                downloadPNG: 'Exportar em PNG',
                downloadSVG: 'Exportar em SVG',
                decimalPoint: ',',
                thousandsSep: '.'
            },
            credits: {
		enabled: false
            },
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: false
            },
            title: {
                text: 'Indicadores Academicos.'
            },
            tooltip: {
                pointFormat: '<b>{series.name}<b>: <b>{point.percentage:.2f}%</b>'
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: true,
                        color: '#000000',
                        connectorColor: '#000000',
                        format: '<b>{point.name}</b>: {point.percentage:.2f} %'
                    }
                }
            },
            series: [{
                type: 'pie',
                name: 'Percentual de Vagas',
                data: [ <?php echo $array_valores; ?> ]
                //data: [ ['Firefox', 45], ['IE', 26], { name: 'Chrome', y: 12, sliced: true, selected: true }, ['Safari', 8], ['Opera', 6], ['Others', 1] ]
            }]
        });
    });
    
</script>

<form id="formulario" name="formulario" method="post" action="">
    <input type="hidden" value="" name="req" id="req" />
    <input type="hidden" value="" name="idaid" id="idaid" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0"> 
        <tr>
            <td>
                <table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem" style="color:333333;">
                    <?php
                        switch($_SESSION['academico']['orgid']) {
                            case '1':
                                $funid = ACA_ID_UNIVERSIDADE;
                                break;
                            case '2':
                                $funid = ACA_ID_ESCOLAS_TECNICAS;
                                break;
                            case '3':
                                $funid = ACA_ID_UNIDADES_VINCULADAS;
                                break;
                        }
                        
                        $sql = "SELECT	    DISTINCT e.entnome,
                                	    	ida.idaid,
                                   			e.entid,
                                    		mdi.imiid as imiid,
                                    		(SELECT COUNT(amiid) FROM academico.arqmedidaindicadores WHERE imiid = mdi.imiid ) AS qtd_arq,
                                    		idaigc,
                                    		idacpcnumcursos,
                                    		idacpcperc	
                            	FROM 		entidade.entidade e
                            	INNER JOIN 	entidade.funcaoentidade ef ON ef.entid = e.entid	
                            	LEFT JOIN 	academico.indicadoresacademicos ida ON ida.entid = e.entid
                            	LEFT JOIN 	academico.medidasindicadores mdi ON mdi.idaid = ida.idaid
                            	LEFT JOIN 	academico.arqmedidaindicadores ami ON ami.amiid = mdi.imiid
	                            WHERE 		e.entstatus = 'A' AND ef.funid = {$funid}
	                            GROUP BY 	e.entnome, ida.idaid, e.entid, mdi.imiid, idaigc, idacpcnumcursos, idacpcperc
	                            ORDER BY 	e.entnome";
                        $campus = $db->carregar($sql);
                        $cabecalho = Array('Campus', 'IGC', 'CPC-N° de Cursos Avaliados', 'Cursos com CPC Menor que 3', 'Medidas Saneadoras', 'QTD. Doc. Anexos');

                        if (is_array($campus)) {
                    ?>
                        <tr>
                            <?php foreach ($cabecalho as $cab) { ?>
                                <td align="center" valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                        <?php echo $cab; ?>
                                </td>
                            <?php } ?>
                        </tr>
                    <?php
                        foreach ($campus as $camp) {
                            /*
                            if ($camp['idaid'] == '') {
                                $sql = "INSERT INTO academico.indicadoresacademicos( entid)
							    VALUES ({$camp['entid']})
							    RETURNING idaid;";
                                $camp['idaid'] = $db->pegaUm($sql);
                                $db->commit();
                            }
                            */ ?>
                            <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                                <td valign="top" title="IFES"><?php echo $camp['entnome']; ?></td>
                                <td valign="top" title="IGC" align="center">
                                    <?php echo $camp['idaigc']; ?>
                                </td>
                                <td valign="top" title="Ação" align="center">
                                    <?php echo $camp['idacpcnumcursos']; ?>
                                </td>
                                <td valign="top" title="Ação" align="center">
                                    <?php
                                        if($camp['idacpcperc'] != ''){
                                            echo str_replace('%', '', $camp['idacpcperc']).' %'; 
                                        } else {
                                            echo '-';
                                        }
                                    ?>
                                </td>
                                <td valign="top" title="Inserir Demanda" align="center">
                                    <img border="0" id="<?php echo $camp['idaid']; ?>"  style="cursor:pointer" class="demanda" src="../imagens/consultar.gif">
                                </td>
                                <td valign="top" title="Inserir Documento" align="center">
                                    <?php echo $camp['qtd_arq']; ?>
                                </td>
                            </tr>
                    <?php
                        }
                    } ?>
                </table>
            </td>
            <td align="center" style="vertical-align: top;">
                <div id="container" style="min-width: 450px; height: 520px; margin: auto 0;"></div>
            </td>
        </tr>
    </table>
</form>