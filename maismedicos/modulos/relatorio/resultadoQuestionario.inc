<?php

include_once "../../global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
header('content-type: text/html; charset=ISO-8859-1');
include APPRAIZ . 'includes/cabecalho.inc'; 
global $db;
monta_titulo('Resultado Questionários', '');

$where = '';
if($_POST['pesquisar']){
    if($_POST['rspnomeinstsupervisora']){
        $where .= " and lower(rspnomeinstsupervisora) = lower('".$_POST['rspnomeinstsupervisora']."') ";
    }
    if($_POST['rspregiaomedico']){
        $where .= " and lower(rspregiaomedico) = lower('".$_POST['rspregiaomedico']."') ";
    }
    if($_POST['rspufmedico']){
        $where .= " and lower(rspufmedico) = lower('".$_POST['rspufmedico']."')";
    }
    if($_POST['rspmunicipiomedico']){
        $where .= " and lower(rspmunicipiomedico) = lower('".$_POST['rspmunicipiomedico']."') ";
    }
}
$sql = "select 
            gpgid,
            gpgdsc,
            prgdsc,
            prg.prgid,
            itpid,
            lower(itpdsc) itpdsc,
            lower(rspdescricao) rspdescricao,
            count(rspid) valores
       from maismedicos.grupopergunta
        join maismedicos.pergunta prg using (gpgid)
        join maismedicos.itempergunta using (prgid)
        join maismedicos.respostaitem rsp using(itpid)
       where gpgid in (1,2,3)
         and itpid not in (2,4,6,8)
         {$where}
        group by 1,2,3,4,5,6,7
        order by 1,2,3,4,5,count(rspid) desc";
 $arrDados = $db->carregar($sql);
 
 ?>
 
<script src="/includes/JQuery/jquery-1.10.2.min.js"></script>
<style type="text/css" media="all">
h2 {
	font-size:14px;
	margin:8px 0 5px 0;
        color: #005580;
	}
p {
	text-align:justify;
	margin:0 0 8px 0;
	}
div.questao {
	width:90%;
	background:#F5F5F5;
	padding:0 10px 5px 10px;
	margin-bottom:1px;
	border:1px dotted #333;
	}
/* CSS para efeito jQuery */
.sm, .fe {
	float:right;
	text-decoration:none;
	color:#069;
	font-size:11px;
	margin-top:2px;
	}
.fe {
	display:none;
	margin-top:10px;
	}
   
</style>
<form name="formulario" id="pesquisar"  method="POST" action="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="25%" class="subtitulodireita">Instituição Supervisora</td>
		<td>
                        <?php
                            $sql = "select distinct 
                                          upper(rspnomeinstsupervisora) as codigo,
                                          upper(rspnomeinstsupervisora) as descricao
                                     from maismedicos.respostaitem
                                    where rspmunicipiomedico is not  null
                                    order by descricao asc";
                            $arrInstituicao = $db->carregar($sql); ?>
                            <select name="rspnomeinstsupervisora"> 
                                    <option value="">Todas as Instituições</option>   
                            <?php foreach($arrInstituicao as $instituicao):
                                        if(($_POST['rspnomeinstsupervisora']) && ($_POST['rspnomeinstsupervisora'] == $instituicao['codigo'])){?>
                                           <option selected='selected' value="<?php echo $instituicao['codigo']; ?>">
                                        <?php }else { ?>
                                           <option value="<?php echo $instituicao['codigo']; ?>">
                                        <?php } echo $instituicao['descricao']; ?>
                                    </option>
                                <?php endforeach; ?>
                            </select> 
    		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Região</td>
		<td>
                    <select id="rspregiaomedico" name="rspregiaomedico">
                         <option value=''>Todas as Regiões</option>
                        <?php $arrRegioes = array('Norte', 'Nordeste', 
                                                   'Sul','Sudeste','Centro-Oeste'); 
                         foreach($arrRegioes as $regiao):
                              if(($_POST['rspregiaomedico']) && ($_POST['rspregiaomedico'] == $regiao)){?>
                                   <option selected='selected' value="<?php echo $regiao; ?>">
                               <?php }else { ?>
                                     <option value="<?php echo $regiao; ?>">
                               <?php } echo $regiao; ?>
                               </option>
                        <?php  endforeach; ?>
                    </select>    
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">UF</td>
		<td>
                   	 <?php
                            $sql = "select distinct 
                                           upper(rspufmedico) as codigo,
                                           upper(rspufmedico) as descricao
                                      from maismedicos.respostaitem
                                     where rspmunicipiomedico is not  null
                                     order by codigo asc";
                            $arrEstados = $db->carregar($sql); ?>
                            <select name="rspufmedico"> 
                                    <option value="">Todos os estados</option>   
                            <?php foreach($arrEstados as $estado):
                                        if(($_POST['rspufmedico']) && ($_POST['rspufmedico'] == $estado['codigo'])){?>
                                           <option selected='selected' value="<?php echo $estado['codigo']; ?>">
                                        <?php }else { ?>
                                           <option value="<?php echo $estado['codigo']; ?>">
                                        <?php } echo $estado['descricao']; ?>
                                    </option>
                                <?php endforeach; ?>
                            </select> 
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Município</td>
		<td id="td_muncod" >
			 <?php
                            $sql = "select distinct 
                                           upper(rspmunicipiomedico) as codigo,
                                           upper(rspmunicipiomedico) as descricao
                                      from maismedicos.respostaitem
                                     where rspmunicipiomedico is not  null
                                     order by codigo asc";
                            $arrMunicipio = $db->carregar($sql); ?>
                            <select name="rspmunicipiomedico"> 
                                    <option value="">Todos os estados</option>   
                            <?php foreach($arrMunicipio as $municipio):
                                        if(($_POST['rspmunicipiomedico']) && ($_POST['rspmunicipiomedico'] == $municipio['codigo'])){?>
                                           <option selected='selected' value="<?php echo $municipio['codigo']; ?>">
                                        <?php }else { ?>
                                           <option value="<?php echo $municipio['codigo']; ?>">
                                        <?php } echo $municipio['descricao']; ?>
                                    </option>
                                <?php endforeach; ?>
                            </select> 
		</td>
	</tr>
	<tr bgcolor="#CCCCCC" >
		<td></td>
		<td>
			<input style="cursor:pointer;" type="submit" value="Pesquisar" />
			<input style="cursor:pointer;" type="button" value="Ver Todos" onClick="window.location=window.location" />
		</td>
	</tr>
</table>
    <input type="hidden" name="pesquisar" value="pesquisar"/> 
</form>
<script type="text/javascript">
//<![CDATA[
	$(document).ready(function() {
		$('.questao *:not(h2.tit)').hide();
		$('.questao h2.tit').append('<a href="#" class="sm">Ver resposta...</a>');	
		$('.questao').prepend('<a href="#" class="fe">Fechar [X]</a>');	
			$('.sm').click(function(event) {
			event.preventDefault();
			$(this).parents('.questao').children().fadeIn(1500);
			$(this).parents('.questao').css('background', '#FFFFFF');
                        $(this).parent().children('.grafico').show();
                        $(this).parent().children('p').show();
			$(this).parents('.questao').siblings('.questao').children('*:not(h2.tit, .sm)').hide();
			$(this).parents('.questao').siblings('.questao').css('background', '#F5F5F5');
			$(this).text('');
			}); 
	
			$('.fe').click(function(event) {
			event.preventDefault();
			$(this).parent().children('*:not(h2.tit, .sm)').hide();
			$(this).parent().css('background', '#F5F5F5');
                        $(this).parent().children('p').hide();
                        $(this).parent().children('.grafico').hide();
			$('.sm').text('Ver resposta...');
			});
                        
            $('#rspnomeinstsupervisora').change(function(){
                 var instituicao = $(this).val();
                    $('#rspregiaomedico').val('');
                    $('#rspufmedico').val('');
                   if(instituicao == ''){
                       $('#rspregiaomedico').removeAttr('disabled');
                       $('#rspufmedico').removeAttr('disabled');
                   }else{
                      $('#rspregiaomedico').attr('disabled','disabled'); 
                      $('#rspufmedico').attr('disabled','disabled'); 
                   }
            });              
	});
   // ]]>
</script>
<?php
if($arrDados != false){

$intCont = 1;
$i_categoria = 0;
foreach ($arrDados as $questoes):
    if ($i_categoria == 0) { ?>
        <fieldset>
            <!-- grupo -->
            <legend style="margin-left:30px"><?php echo $questoes['gpgdsc']; ?></legend>
        <?php $i_questao = 0; }
        if ($arrDados[$intCont]['gpgdsc'] != $questoes['gpgdsc']) {
            $i_categoria = 0;
            $ultimaCategoria = true;
        } else {
            $i_categoria = 1;
            $ultimaCategoria = false;
        }
       
        if($i_questao == 0){ ?>
        <!-- inicio questao -->    
          <div class="questao" style="margin-left:30px">
             <h2 class="tit"><?php echo $questoes['prgdsc']; ?></h2></br>     
       <?php 
             $i_item = 0;
              } 
            if ($arrDados[$intCont]['prgdsc'] != $questoes['prgdsc']) {
                $i_questao = 0;
                $ultimaQuestao = true;
              }else{
                $i_questao = 1;
                $ultimaQuestao = false; 
              } 
              
            if($i_item == 0){
                  $idGrafico = 'grafico'.$intCont;
                  $contGrafico = 0; ?>
                 <p style="font-weight: bold; display: none;"><?php echo ucfirst($questoes['itpdsc']); ?></p>
                 <div id="<?php echo $idGrafico; ?>" name="<?php echo $idGrafico; ?>" style="width: 90%; min-height: 300px; display: none;"></div>
            <?php } 
                 if($questoes['rspdescricao'] == ''){
                              $questoes['rspdescricao'] = 'nulo';   
                             }
                             if($questoes['valores'] == ''){
                                 $questoes['valores'] = 0;
                             }
                       $questoes['rspdescricao'] = str_replace("'", "", $questoes['rspdescricao']);       
                  if($arrDados[$intCont]['itpdsc'] != $questoes['itpdsc']){
                      $contGrafico++;
                      $arrGrafico[$contGrafico]['descricao'] = $questoes['rspdescricao'];
                      $arrGrafico[$contGrafico]['valor'] = $questoes['valores'];  
                     
                      $total = 0;
                      foreach($arrGrafico as $grafico ){
                          $total += $grafico['valor']; 
                      }
                      ?>
                  <script type="text/javascript">
                         jQuery(function () {
                            jQuery('#<?php echo $idGrafico; ?>').highcharts({
                                chart: {
                                    plotBackgroundColor: null,
                                    plotBorderWidth: null,
                                    plotShadow: false
                                },
                                title: {
                                    text: ''
                                },
                              tooltip: {
                                 formatter: function() {
                                               return '<b>'+ this.point.name +'</b><br/>'+
                                                       '<b>Votos: </b>'+ this.y+' de '+this.total;
                                            } 
                                    
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
                                        },
                                          showInLegend: true,
                                    }
                                },
                                series: [{
                                    type: 'pie',
                                    name: 'Percentual',
                                    data: [
                                       <?php foreach($arrGrafico as $grafico ){ ?>
                                         {
                                            name: '<?php echo $grafico['descricao']; ?>',
                                            y: <?php echo $grafico['valor']; ?>,
                                            total: <?php echo $total; ?>,
                                        }, 
                                       <?php } ?>         
                                    ]
                                    }]
                                });
                            jQuery('tspan:last').hide();
                         });
		       </script> 
                  <?php  $i_item = 0;
                         unset($arrGrafico);
                         }else{ 
                            $arrGrafico[$contGrafico]['descricao'] = $questoes['rspdescricao'];
                            $arrGrafico[$contGrafico]['valor'] = $questoes['valores'];  
                            $contGrafico++;
                           $i_item = 1;
                        } ?>     
             
       <!-- ultima questao -->
      <?php if ($ultimaQuestao == true) { echo "</div>"; } ?>     
  <?php if ($ultimaCategoria == true) { echo "</fieldset><br/>"; }  
        $intCont ++; endforeach; 
}else{ ?>
  <h5 style="margin-left:400px; color:red;">Nenhum resultado encontrado para os parâmetros informados</h5>      
<?php } ?>       
<script src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>