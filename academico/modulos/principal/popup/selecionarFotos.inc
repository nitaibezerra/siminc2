<?php

/**
 * Arquivo que seleciona as fotos da galeria para o Extrato da obra
 * 
 * @author Fernando Araújo Bagno da SIlva
 * @since 11/02/2010
 * 
 */

$obrid = $_REQUEST['obrid'];

?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

	<?php 
	
		$sql = "SELECT 
					arq.arqid, 
					arq.arqdescricao,
					to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao
				FROM 
					public.arquivo arq
				INNER JOIN 
					obras.arquivosobra oar ON arq.arqid = oar.arqid
				INNER JOIN 
					obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
				WHERE 
					obr.obrid = {$obrid} AND
					aqostatus = 'A' AND
					(arqtipo = 'image/jpeg' OR 
					 arqtipo = 'image/gif' OR 
					 arqtipo = 'image/png') 
				ORDER BY
					arq.arqid DESC";
		
		$fotosObras = ($db->carregar($sql));
		
		if ( $fotosObras ){
			
			print "<tr>";
			
			$j = 1;
			for( $i = 0; $i < count($fotosObras); $i++ ){
				
				print "    <td align='center'>"
					. "        <img src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=100&newheight=100&arqid={$fotosObras[$i]["arqid"]}' hspace='3' vspace='3' style='width:50px; height:50px; '\n>"
					. "        <br/>{$fotosObras[$i]["aqodtinclusao"]}"
					. "        <br/><input type='checkbox' name='fotos[]' id='{$fotosObras[$i]["arqid"]}' onclick='salvaFotosSelecionadas(this, {$fotosObras[$i]["arqid"]})' />"
					. "    </td>";
				
				if( is_int($j/5) ){
					print "</tr><tr>";
				}
				$j++;
			}
			print "</tr>";
			 
		}else{
			
			print "<tr><td align='center' style='color:#ee0000'> Não existem fotos cadastradas para esta obra. </td></tr>";
			
		}
		
	?>
	</table>
