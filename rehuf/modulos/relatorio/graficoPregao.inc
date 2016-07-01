<?php

 if($_REQUEST['tipo'] == 1){
     $titulo = 'Participação dos HUs no Pregão';
   }else{
      $tituloGrafico = 'Comparativo de Participação do HU nos Pregões';
      $titulo = 'Participação do HU nos Pregões';
   }
   
//Cabecalho
include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
global $db; 
print '<br/>';
monta_titulo( $titulo, "" );
$preid_dsc = 'Selecione..';
$entid_dsc = 'Selecione..';
$descricao = '';
extract($_POST);

if($_POST)
{
      if($tipo == 1)
      {
          unset($_REQUEST['entid']);  
          $descricao = $preid_dsc;
      }else{
          unset($_REQUEST['preid']);  
          $descricao = $entid_dsc;
      }
  
   $sql        = monta_sql();
   $dadosPregao = $db->carregar($sql);
   $entidades   =  lista_hospitais();
   $arrGrafico       = array();  
  
 if($dadosPregao[0] && $entidades[0])
{
  /* Loop de pregoes selecionados e concatencao jquery(style) */
  $i = 0;
  foreach($dadosPregao as $key1 => $pregao):
     $nomePregao = $pregao['itedescricao'];
     $quantidade = $pregao['nitempregao'];
     /* Loop entidades */
     foreach ($entidades as $entidade) 
     {
        $ippquantidade = qtdItensprenchidos_HospitalxPregao($entidade['entid'], $pregao['preid'] );
       
        if($ippquantidade)
        {
            $hospabrv = '';
            $resumoent = explode(" ", $entidade['entnome']);
            $porcent = round(($ippquantidade*100)/$pregao['nitempregao'],1);
            for($j=0;$j<count($resumoent);$j++) { 
               $hospabrv .= strtoupper(substr(trim($resumoent[$j]),0,1)); 
            } 
            $hospital = (($entidade['entsig'])?$entidade['entsig']."-":"").$hospabrv;
            $arrGrafico[$i]['preid'] =  $pregao['preid'];
            $arrGrafico[$i]['itedescricao'] =  $nomePregao;
            $arrGrafico[$i]['entnome'] =  $hospital;
            $arrGrafico[$i]['nitempregaohu'] =  $ippquantidade;
            $arrGrafico[$i]['porcitempregaohu'] =  $porcent.'%';
            $arrGrafico[$i]['porcitempregaohu2'] =  $porcent;
            $arrGrafico[$i]['itenspregao'] =  $quantidade;
            $i++;        
        }
         
     }
    endforeach;  
  }
}

function lista_hospitais()
{
    global $db;
    extract($_REQUEST);
    
    if(strlen($entid) < 3){
      $permissoes = verificaPerfilRehuf();
       if(count($permissoes['verhospitais']) > 0) {
         $sqlHospitais = "SELECT ent.entid as codigo FROM entidade.entidade ent 
                             LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
                             LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
                             LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                             LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
                             WHERE fen.funid = '". HOSPITALUNIV ."' 
                               AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
                               AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') 
                               AND ent.entnome||' - '||coalesce(ena.entsig, '') = '{$descricao}'
                             ORDER BY ena.entsig";
           $entid = $db->pegaUm($sqlHospitais);                    
         }    
    }
    
    if($entid){
         $entidade = " AND ent.entid in ($entid) ";
     }else{
         $entidade = '';
     }
    
    $sql = "SELECT ent.entnome, ena.entsig, ent.entid, esd.esddsc FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
		LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
		LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
                INNER JOIN workflow.documento doc ON doc.docid = esu.docid 
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		WHERE fen.funid IN ('". HOSPITALUNIV ."','".HOSPITALFEDE."') 
                AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
                {$entidade}
		ORDER BY ena.entsig";
   
    return  $db->carregar($sql);
}

function monta_sql($retornaQuery = NULL)
{
	global $db;
        $arWhere = array();
	extract($_REQUEST);
	
        
        $arWhere[] = " g.grustatus = 'A' ";
        $arWhere[] = " ig.itgstatus = 'A' ";
        $arWhere[] = " igp.igpstatus = 'A' ";
        $arWhere[] = " pre.prestatus = 'A' ";
      
        if($preid){
          // $preid = implode(",",$preid);
           $arWhere[] = " igp.preid in ($preid) ";
        }
        
        $sql = "SELECT  count(1) as nitempregao,
                        igp.preid,
                        pre.preobjeto||' ('||COALESCE(pre.precodigo,'-')||')' as itedescricao
                  FROM 
                        rehuf.item i 
			inner join rehuf.itemgrupo ig 
			  	on (i.iteid = ig.iteid) 
			inner join rehuf.grupoitens g
			  	on (ig.gruid = g.gruid)
			inner join rehuf.itemgrupopregao igp
    			        on (ig.itgid = igp.itgid) 
                        inner join rehuf.pregao pre
                                on pre.preid = igp.preid
    		  left join rehuf.itemlote il 
    		  	on (il.itlid = i.itlid)
			WHERE i.itestatus = 'A'
			AND g.grustatus = 'A'
			AND ig.itgstatus = 'A'
			AND igp.igpstatus = 'A'
			".( !empty($arWhere) ? ' AND' . implode(' AND ', $arWhere) : '' ).
			" GROUP BY igp.preid, pre.preobjeto||' ('||COALESCE(pre.precodigo,'-')||')'  
                         ORDER BY igp.preid, pre.preobjeto||' ('||COALESCE(pre.precodigo,'-')||')'";
         $itens = $db->carregar($sql);
      return $sql;
}

function qtdItensprenchidos_HospitalxPregao( $entid, $preid)
{
    global $db;
    $sql = "SELECT count(1) FROM rehuf.itenspregaopreenchido ipp 
	    INNER JOIN rehuf.huparticipante hup ON hup.hupid=ipp.hupid 
	    WHERE entid='".$entid."' AND preid='".$preid."' AND ippquantidadeanual is not null";
  
    return $db->pegaUm($sql);
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script src="/includes/Highcharts-4.0.3/js/highcharts.js"></script>
<script src="/includes/Highcharts-4.0.3/js/modules/exporting.js"></script>
<script type="text/javascript">
    $(document).ready(function(){
      $('#preid_dsc').val('<?=$preid_dsc;?>');  
      $('#entid_dsc').val('<?=$entid_dsc;?>');  
    });
    
    function validateForm(){
        var tipo = $('#tipo').val();
        
        if(tipo == 1){
            if($('#preid').val() == ''){
                alert('Selecione o Pregão.');
                return false;
            }
        }else{
            if($('#entid').val() == ''){
                alert('Selecione o Hospital.');
                return false;
            }  
        }
         return true;
      }
</script>    
<form action="" method="post" name="formulario"  onsubmit="return validateForm();">
	<input type="hidden" name="tipo" id="tipo" value="<?=$_REQUEST['tipo'];?>"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">	
            <?php if($_REQUEST['tipo'] == 1){ ?>
        	<tr>
			<td class="SubTituloDireita" valign="top">Pregão :</td>
			<td id="td_pregao">
			<?
				$sql = "SELECT preid as codigo, preobjeto||' ('||COALESCE(precodigo,'-')||')' as descricao FROM rehuf.pregao WHERE prestatus='A'";
				campo_popup('preid',$sql,'Selecione..','','400x400',100);
			?>
                            <img src="../imagens/obrig.gif">  
			</td>
		</tr>
            <?php }else{ ?>
               <tr>
			<td class="SubTituloDireita" valign="top">Hospital :</td>
			<td id="td_hospitais">
			<?
			$permissoes = verificaPerfilRehuf();
			if(count($permissoes['verhospitais']) > 0) {
				$sqlHospitais = "SELECT ent.entid as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM entidade.entidade ent 
								 LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
								 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
								 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') 
								 ORDER BY ena.entsig";
				campo_popup('entid',$sqlHospitais,'Selecione..','','400x400',80);
				
			} else {
				echo "Seu perfil não possui hospitais";	
			}
			?>
                         <img src="../imagens/obrig.gif">     
			</td>
		</tr> 
            <?php } ?>    
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left; padding-top: 10px;">
                    <input type="submit" value="Gerar Gráfico">
		</td>
	</tr>
   </table>
</form>
 
<?php        
if(($_POST)&&(!$arrGrafico)){ ?>
<center>
	<h3>
		<b>Nenhum resultado para os par&acirc;metros informados</b>
	</h3>
</center>

<?php  }else{ // Nenhum resultado ?> 
<?
  /* Evita barra em branco caso a quantidade de colunas seja grande */
  $arrColor[] = '#2F7ED8'; $arrColor[] = '#0D233A'; $arrColor[] = '#8BBC21'; $arrColor[] = '#910000';
  $arrColor[] = '#1AADCE'; $arrColor[] = '#624289'; $arrColor[] = '#F28F43'; $arrColor[] = '#77A1E5';
  $arrColor[] = '#C42525'; $arrColor[] = '#A6C96A'; $arrColor[] = '#2F7ED8'; $arrColor[] = '#263C53';
  $arrColor[] = '#8BBC21'; $arrColor[] = '#910000'; $arrColor[] = '#1AADCE'; $arrColor[] = '#2F7ED8';
  $arrColor[] = '#0D233A'; $arrColor[] = '#8BBC21'; $arrColor[] = '#910000'; $arrColor[] = '#1AADCE';
  $arrColor[] = '#624289'; $totalCor = count($arrColor);
?>
<div id="container" name="grafico"
style="margi-top: 120px; margin-left: 20px; margin-right: 30px; min-width: 500px; min-height: 800px;"></div>

<?php if($tipo == 1){  ?>
 <script type="text/javascript">
  $(function () {
    
        var colors = Highcharts.getOptions().colors,
            categories = [<?php
		$cont = 0;
                $contArray = count($arrGrafico);
		foreach ( $arrGrafico as $grafico ) :
			?>
                             '<?= $grafico['entnome']; ?>'
                             <?= ($cont != $contArray) ? ',' : ''; ?>
                    <?php $cont ++;
                endforeach;?>],
            name = 'Total de Participantes: <?= formata_numero($cont,2);?>',
               data = [<?php
                    $cont = 0;
                    $contCor = 0;
		foreach ( $arrGrafico as $grafico ) : ?>
                          {
                              //  y: <?php //echo (int)$grafico['nitempregaohu']; ?>,
                              //  porcent: '<?php // echo $grafico['porcitempregaohu']; ?>',
                                color:' <?= $arrColor[$contCor]; ?>',
                                y: <?= (int)$grafico['porcitempregaohu']; ?>,
                                porcent: '<?= $grafico['porcitempregaohu']; ?>',
                                quantidade: '<?= (int)$grafico['nitempregaohu']; ?>',
                                color:' <?= $arrColor[$contCor]; ?>',
                                name: '<?= $grafico['entnome']; ?>'
                          }
                            <?= ($cont != $contArray) ? ',' : ''; ?>
                         <?php
                              $contCor++;
                              if($contCor == $totalCor){ $contCor =0; }
			      $cont ++;
                endforeach;
                   ?>];
    
        function setChart(name, categories, data, color) {
			chart.xAxis[0].setCategories(categories, false);
			chart.series[0].remove(false);
			chart.addSeries({
				name: name,
				data: data,
				color: color || 'white'
			}, false);
			chart.redraw();
        }
    
        var chart = $('#container').highcharts({
            chart: {
                type: 'column'
            },
            title: {
                text: 'Comparativo de Participação dos HUs no Pregão'
            },
            subtitle: {
                text: '<h3><?= $descricao; ?> / Quantidade de Itens: <?= $arrGrafico[0]['itenspregao']; ?></h3>'
            },
            xAxis: {
                  labels: {
                    rotation: -45
                 },
                categories: categories
            },
            yAxis: {
                max: 100,
                title: {
                    text: 'Participação do HU no Pregão (%)'
                }
            },
            plotOptions: {
                column: {
                    cursor: 'pointer',
                    point: {
                        events: {
                            click: function() {
                                var drilldown = this.drilldown;
                                if (drilldown) { // drill down
                                    setChart(drilldown.name, drilldown.categories, drilldown.data, drilldown.color);
                                } else { // restore
                                    setChart(name, categories, data);
                                }
                            }
                        }
                    },
                    dataLabels: {
                        enabled: true,
                        color: '#443E3E',
                         style: {
                            fontWeight: 'bold'
                        },
                       formatter: function() {
                                   var point = this.point,
                                s = point.y;
                            return s;
                       }
                    }
                }
            },
            tooltip: {
                        formatter: function() {
                            var point = this.point,
                                s = this.x +'<br/><b>Participação: '+ 
                                point.porcent +'</b><br/><b>Itens Preenchidos: '+
                                point.quantidade+' de  <?= $arrGrafico[0]['itenspregao']?></b>';
                            return s;
                        }
            },
            series: [{
                name: name,
                data: data,
                color: 'white'
            }],
            exporting: {
                enabled: false
            }
        })
        .highcharts(); // return chart
       $('.highcharts-tooltip:last').next().hide();
    }); 
</script>     
<?php   }// fim pregao ?>


<?php   if($tipo == 2){
 foreach ($arrGrafico as $key => $row) {
    $esddsc[$key] = $row['itedescricao'];
    $tempo[$key] = $row['porcitempregaohu2'];
  }
  array_multisort($tempo, SORT_DESC, $esddsc, SORT_ASC, $arrGrafico);
        ?>
<script type="text/javascript">
            $(function() {
                $('#container').highcharts({
                    chart: {
                        plotBackgroundColor: null,
                        plotBorderWidth: null,
                        plotShadow: false
                    },
                    title: {
                        text: 'Gráfico Comparativo de Participação do HU nos Pregões'
                    },
                    subtitle: {
                        text: '<h3><?= $descricao; ?></h3>'
                    },
                    tooltip: {
                        formatter: function() {
                              var point = this.point,
                                s = point.name +'<br/><b>Participação: '+ 
                                point.porcent +'</b><br/><b>Itens Preenchidos: '+
                                point.qtdHu+' de  '+point.qtdPregao+'</b>';
                            return s;
                        },
                    },
                    plotOptions: {
                        pie: {
                            allowPointSelect: true,
                            cursor: 'pointer',
                            dataLabels: {
                                enabled: true,
                                color: '#000000',
                                connectorColor: '#000000',
                                format: '<b>{point.name}</b>: {point.porcent}'
                            },
                            showInLegend: true,
                        }
                    },
                    legend: {
                        useHTML: true,
                        align: 'center',
                        width: 1200,
                        itemWidth: 600,
                        labelFormatter: function() {
                            return  ''+this.name;
                        }
                    },
                    series: [{
                            type: 'pie',
                            name: 'Percentual',
                            data: [ <?php $contCor = 0;
                                     foreach ( $arrGrafico as $i => $grafico ) : ?>
                                    {
                                        name: '<?php echo $grafico['itedescricao']; ?>',
                                        y: <?php echo $grafico['porcitempregaohu2']; ?>,
                                        qtdHu: <?php echo $grafico['nitempregaohu']; ?>,
                                        qtdPregao: <?php echo $grafico['itenspregao']; ?>,
                                        porcent: '<?php echo $grafico['porcitempregaohu']; ?>',
                                        color:' <?= $arrColor[$contCor]; ?>'
                                    },
                              <?php  $contCor++;
                                     if($contCor == $totalCor){ $contCor =0; }
                               endforeach; ?>
                            ]
                        }],
                  exporting: {
                      enabled: false
                  }
                });
                 $('.highcharts-tooltip:last').next().hide();
            }); 
        </script>
<?php     }// fim hu  ?>
<?php }//graficos ?>
                