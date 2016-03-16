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


$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Plano de Ação - Parcela Complementar', '');
echo cabecalhoPDE(); 


        $sql = "SELECT 
        			pasp.pasid,
					osp.ospdescricao,
					esp.espdescricao,
					msp.mspdescricao
				FROM
					pdeescola.objetivosegundaparcela osp
				INNER JOIN
					pdeescola.especificacaosegundaparcela esp ON esp.ospid = osp.ospid
				INNER JOIN
					pdeescola.metasegundaparcela msp ON msp.espid = esp.espid
				INNER JOIN
					pdeescola.acaosegundaparcela p ON p.mspid = msp.mspid
				INNER JOIN 
					pdeescola.planoacaosegundaparcela pasp ON pasp.aspid = p.aspid
				LEFT JOIN 
					pdeescola.tiporecurso tr ON tr.treid = pasp.treid
				WHERE
					pdeid = ".$_SESSION['pdeid'];
        //dbg($sql,1);
		$rsDados = $db->carregar($sql);
		//dbg($rsDados,1);
		$rsDados = $rsDados ? $rsDados : array();
		foreach( $rsDados as $arDados ){
			//if($rsDados['pasid']){
						$msg = "Não informado";
						
		        		$ospdescricao		 = $arDados['ospdescricao'] ? $$arDados['ospdescricao'] : $msg ;
						$espdescricao 		 = $$arDados['espdescricao'] ? $$arDados['espdescricao'] : $msg ;
						$mspdescricao   	 = $$arDados['mspdescricao'] ? $$arDados['mspdescricao'] : $msg ;
						$aspdescricao   	 = $$arDados['aspdescricao'] ? $$arDados['aspdescricao'] : $msg ;
?>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script src="../includes/prototype.js"></script>
	    
		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
			<tr>
				<td class="subtitulodireita" width="20%"><b>Objetivo:</b></td> 
				<td align="left"><?php echo $arDados['ospdescricao'];?></td>
            </tr>	
 			<tr>
				<td class="subtitulodireita"><b>Estratégia:</b></td> 
				<td align="left"><?php echo $arDados['espdescricao']; ?> </td>
			</tr>	 
			<tr>
				<td class="subtitulodireita"><b>Meta:</b></td> 
				<td align="left"><?php echo $arDados["mspdescricao"]; ?> </td>
			</tr>	 
	
		</table>
			
		 <?php
		 
		        	$sql1 = "SELECT
		        				p.aspdescricao as acao,
								pasp.pasdescricaoitem as descricao,
								tr.tredescricao as natureza,
								pasp.pasquantidade as quantidade,
								to_char(pasvalorunitario, '999g999g999g990d99') as unitario,
								to_char((pasp.pasquantidade * pasp.pasvalorunitario), '999g999g999g990d99') as total
							FROM
								pdeescola.objetivosegundaparcela osp
							INNER JOIN
								pdeescola.especificacaosegundaparcela esp ON esp.ospid = osp.ospid
							INNER JOIN
								pdeescola.metasegundaparcela msp ON msp.espid = esp.espid
							INNER JOIN
								pdeescola.acaosegundaparcela p ON p.mspid = msp.mspid
							INNER JOIN 
								pdeescola.planoacaosegundaparcela pasp ON pasp.aspid = p.aspid
							LEFT JOIN 
								pdeescola.tiporecurso tr ON tr.treid = pasp.treid
							WHERE 
								pasid = ".$arDados['pasid']."
							ORDER BY 
								pasp.pasdescricaoitem asc
							";
		        	
		        	$cabecalho = array("AÇÃO","&nbsp;&nbsp;&nbsp;DESCRIÇÃO", "NATUREZA DA DESPESA", "QUANTIDADE", "VALOR UNITÁRIO", "VALOR TOTAL" ); 
	    			$db->monta_lista_simples( $sql1, $cabecalho,'25','1' ,'N', '', '','');    
//					if($pasid != '' ){			
//					$dados = $db->carregar( $sql );
					//}
		     	?><!--
		
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
		<thead>
		<tr>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Nº
			</td> 
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				DESCRIÇÃO
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				NATUREZA DA DESPESA
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				QUANTIDADE
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				VALOR UNITÁRIO
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				VALOR TOTAL
			</td>
		</tr> 
		</thead>
		<tbody>
		
	<?php
	//loop preenche Desdobramento das Metas em Planos de Ação
	//for( $k = 0; $k < count( $dados ); $k++ ){
		//tratando lista caso não exista.
		//if ($dados){
			?>
			<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
				<td title="Descrição">
					<b><?//= $k+1 ?></b>
				</td> 
				<td title="Ação">
					<?//=$dados[$k]['acao']?>
				</td>
				<td title="Descrição">
					<?//=$dados[$k]['descricao']?>
				</td>
				<td title="Natureza da despesa">
					<?//=$dados[$k]['natureza']?>
				</td>
				<td title="Quantidade">
					<?//=$dados[$k]['quantidade']?>
				</td>
				<td title="Valor Unitário">
					<?//=$dados[$k]['unitario']?>
				</td>
				<td title="Total">
					<?//=$dados[$k]['total']?>
				</td>
			</tr>
		<?php
					//}else{
						//echo "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' style='color:333333;' class='listagem'>
								//<thead>
									//<tr>
										//<td style='color:red;' align='center';>
											//Não foram encontrados Registros.
										//</td>
									//</tr>
							  //</table>";
						//continue;	 	
		 			//}//fim else
				//}//fim do for
		?>
			</tbody>
			</table>
			--><br>
			<br>
			<br>
		<?//}
	    //}
  	  //}
//	}else{
//		die('<script>
//			 alert(\'Não foram encontrados Registros.\');
//			 history.go(-1);
//	 		 </script>');
//    }
}
?>

		<div align="center">
			<input type="button" value="Voltar" onClick="location.href='pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A'" />
		</div>
         
