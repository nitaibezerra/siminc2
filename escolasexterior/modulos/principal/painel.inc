<script type="text/javascript" src="/includes/prototype.js"></script>	

<?

//Monta o cabeçalho e título da tela
include  APPRAIZ."includes/cabecalho.inc";
echo"<br>";
monta_titulo("Painel", "");

?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
		<tr>
		 <th>
		 	<a style="cursor:pointer;" onclick="window.location.href='/escolasexterior/escolasexterior.php?modulo=painel&acao=A'">Escolas no Exterior - Ano <?php echo $_SESSION['exercicio'];?></a>
		 </th>
		</tr>
		<tr>
			<td valign="top">
				<?php
				
				    $sql = "select ed.esdid, ed.esddsc, count(eexid) as count
								from workflow.estadodocumento ed
								LEFT JOIN 
								workflow.documento d on d.esdid=ed.esdid 
								LEFT JOIN 
								escolasexterior.escolasexterior e on e.docid=d.docid
								where ed.tpdid=".WF_TPDID_EE."
								group by ed.esddsc, ed.esdordem, ed.esdid
								ORDER BY ed.esdordem
							";

					$link = array("link" => "em.php?modulo=lista&acao=E&modalidade=M&emianoreferencia=".$ano."", "campo" => "esddsc", "get" => "esdid");
					
					
					$rsEstado = $db->carregar( $sql ); 
	
				 	$tabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';    
					 if(!$rsEstado){
					 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
					 	echo $tabela; 
					 	return false;
					 }else{
					 	
					 	for( $i =0; $i<count( $rsEstado ); $i++ ){
					 		//$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
					 		$seta_filho = " <img  src=\"../imagens/mais.gif\" /> ";
				 		 	($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
					 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
					 		$tabela .= "	<td width =\"80%\"> ".$seta_filho."<a href=\"escolasexterior.php?modulo=principal/lista&acao=A&esdid=".$rsEstado[$i]['esdid']."\">".$rsEstado[$i]['esddsc']."</a></td>";
					 		$tabela .= "	<td style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
					 		$tabela .= "</tr>"; 
					 	}
					 	$tabela .= "</table>";
					 }
					echo $tabela;
					
				?>
			</td>	
		</tr>
	</table>