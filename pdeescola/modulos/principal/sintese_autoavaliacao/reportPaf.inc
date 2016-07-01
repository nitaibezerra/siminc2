<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();
 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid 	= $_SESSION["pdeid"];
$entid  = $_SESSION["entid"];
$pseid = $_REQUEST['pseid'];

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}
//query que carrega os dados do subcabecalho [objetivo...]


$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Plano de Ação ', '');
echo cabecalhoPDE(); 
	  
$sql = "select 
			d.tcaid,
			t.tcadescricao, 
			coalesce( sum( d.dpavalorcapital ), 0 ) as capital,
			coalesce( sum( d.dpavalorcusteio ), 0 ) as custeio 
		from pdeescola.detalheplanoacao as d 
		inner join pdeescola.tipocategoria as t on t.tcaid = d.tcaid
		where d.forid = 5 and plaid in ( 
			select 
				plaid
				from pdeescola.planoacao
			where pseid in ( 
				select 
				pseid
				from pdeescola.planosuporteestrategico as pl 
				where  pl.pdeid = '{$_SESSION['pdeid']}'
				)
		) 
		group by d.tcaid, t.tcadescricao
		";
		$dados = $db->carregar( $sql ); 

?>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
	<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
   
   			<tr>
   				<td colspan="3">
   					<div class="SubtituloEsquerda" style="padding: 5px; font-size: 17px; text-decoration: none; text-align: center">PAF</div>
   				</td>
   			</tr>
          
			<tr>
				<th width="50%">
					<b>	Categoria</b>
				</th> 
				<th align="left">
					 <b>R$ Custeio</b>
                </th>
                <th align="left">
					<b>R$ Capital</b>
                </th>
            </tr>
            <?
            if( $dados ){
	            
				foreach( $dados as $d ){	
	            	  
						 $totCapital += $d['capital'];
						 $totCusteio += $d['custeio'];
						 ?>	
		            <tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
						<td>
							<?=$d['tcadescricao']; ?>
						</td> 
						<td align="left" style="color:#3399FF;" title="Custeio (R$)">
							<?=number_format($d['custeio'],2,',','.'); ?>
		                </td>
		                <td align="left" style="color:#3399FF;" title="Capital (R$)">
							<?=number_format($d['capital'],2,',','.'); ?>
		                </td>
		            </tr>
		            <?
					  
				}//end foreach	 
	           ?>	 
	            <tr height="5px"bgcolor="silver" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='silver';">
						<td>
							<b>Total</b>
						</td> 
						<td align="left" style="color:#3399FF;" title="Custeio (R$)">
							<?=number_format($totCusteio,2,',','.'); ?>
		                </td>
		                <td align="left" style="color:#3399FF;" title="Capital (R$)">
							<?=number_format($totCapital,2,',','.'); ?>
		                </td>
		            </tr>
		      <?
              }else{
              	?>
              	 <tr height="5px"bgcolor="silver" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='silver';">
						<td colspan="3">
							 <center> não foram encontratos registros</center>
						</td> 
						 
		            </tr> 
              	<?
              }
		      ?> 
	       </table>
	       	<div align="center">
		<input type="button" value="Voltar" onClick="location.href='pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A'" />
	</div>