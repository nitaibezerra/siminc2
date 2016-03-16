<?php
header('content-type: text/html; charset=ISO-8859-1');
echo "<br>";
monta_titulo( "Solicitação de Diária", '' );

function caixaComplemento( $solid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT	
				val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)) as valorcomp,
				valorant,
				val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)) - valorant as valorpagar
			FROM 
				evento.solicitacaodiaria sol
			INNER JOIN evento.itinerariosolicitacaodiaria iti ON iti.solid  = sol.solid AND iti.itistatus = 'A'
			INNER JOIN (
				SELECT DISTINCT
					solid,
					sum(
						(itidias*(itivalordiaria*(CASE WHEN itimeiadiaria THEN 0.5 ELSE 1 END)
							      )
						)+
						(itivaloradicionalembarque
						)+
						(CASE WHEN itiultimo THEN (itivalordiaria*0.5) ELSE 0 END
						)+
						(itidiasviajem*itivalordiaria)
					) as valor
				FROM
					evento.itinerariosolicitacaodiaria
				WHERE
					itistatus = 'A'
				GROUP BY solid) val ON val.solid = sol.solid
			INNER JOIN (
					SELECT DISTINCT	
						val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)) as valorant,
						sol.solid
					FROM 
						evento.solicitacaodiaria sol
					INNER JOIN evento.itinerariosolicitacaodiaria iti ON iti.solid  = sol.solid AND iti.itistatus = 'A'
					INNER JOIN (
						SELECT DISTINCT
							solid,
							sum(
								(itidias*(itivalordiaria*(CASE WHEN itimeiadiaria THEN 0.5 ELSE 1 END)
									      )
								)+
								(itivaloradicionalembarque
								)+
								(CASE WHEN itiultimo THEN (itivalordiaria*0.5) ELSE 0 END
								)+
								(itidiasviajem*itivalordiaria)
							) as valor
						FROM
							evento.itinerariosolicitacaodiaria
						WHERE
							itistatus = 'A'
						GROUP BY solid) val ON val.solid = sol.solid ) ant ON ant.solid = sol.solcomplemento
			WHERE
				sol.solid = ".$solid;
	$info = $db->pegaLinha($sql);
	
	
	?>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" width="100%" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloCentro" colspan="2">Complemento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor Complemento</td>
		<td>
			R$ <?=number_format($info['valorcomp'],2,',','.'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="50%">Valor Complementado</td>
		<td style="color:red;border-bottom: 1px solid black;">
			- R$ <?=number_format($info['valorant'],2,',','.'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor a pagar</td>
		<td>
			R$ <?=number_format($info['valorpagar'],2,',','.'); ?>
		</td>
	</tr>
</table>
<br />
	<?php 
}

if( $_REQUEST['solid'] ){
	
	$sql = "SELECT 
				usucpf, 
				solnumdocestrangeiro, 
				solnome, 
				solemail, 
				solbanco, 
				solagencia,  
				solconta||' ' as conta,  
				solcodigoscdb, 
				solobservacao,
				solobjetivo,
				soldeducaoalim,
				round(soldeducaotransp*soltotdias,2) as soldeducaotransp,
				soltotdias,
				solcomplemento,
				docid
			FROM 
				evento.solicitacaodiaria 
			WHERE
				solstatus = 'A'
				AND solid = ".$_REQUEST['solid'];
	
	$solicitacao = $db->pegaLinha($sql);
	
	$sql = "SELECT
				CASE WHEN itiescala
					THEN 'Escala'
					ELSE ''
				END as escala,
				itiultimo,
				'<div align=\"center\">'||
				mun1.muncod|| ' - ' || mun1.mundescricao||
				'<br />'||
				to_char(itidataorigem,'DD/MM/YYYY')||
				'</div>' as origem,
				'<div align=\"center\">'||
				mun2.muncod|| ' - ' || mun2.mundescricao||
				'<br />'||
				to_char(itidatadestino,'DD/MM/YYYY')||
				'</div>' as destino,
				itidias,
				itivalordiaria, 
				itidiasviajem*itivalordiaria as valorper, 
				itivaloradicionalembarque as emb,
				(
					(iti.itidias*(iti.itivalordiaria*(CASE WHEN iti.itimeiadiaria THEN 0.5 ELSE 1 END)
						      )
					)+
					(CASE WHEN iti.itiultimo THEN (iti.itivalordiaria/2) ELSE 0 END
					)
				) as valorViagem,
				(
				(iti.itidias*(iti.itivalordiaria*(CASE WHEN iti.itimeiadiaria THEN 0.5 ELSE 1 END)
					      )
				)+
				(iti.itivaloradicionalembarque
				)+
				(CASE WHEN iti.itiultimo THEN (iti.itivalordiaria/2) ELSE 0 END
				)
			) as valor,
			itimeiadiaria
		FROM 
			evento.itinerariosolicitacaodiaria iti
		LEFT JOIN evento.valordiaria     vld ON vld.vldid = iti.vldid
		INNER JOIN territorios.municipio mun1 ON mun1.muncod = itiorigem 
		INNER JOIN territorios.municipio mun2 ON mun2.muncod = itidestino 
		WHERE
			itistatus = 'A'
			AND solid =".$_REQUEST['solid'];

	$viagem = $db->carregar($sql);
	
	$cpfShow 	= $solicitacao['usucpf'] != ''		   		 ? '' : 'display:none';
	$numdocShow = $solicitacao['solnumdocestrangeiro'] != '' ? '' : 'display:none';
}


?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript">
		
		jQuery.noConflict();
		
		jQuery(document).ready(function() {
			
			var totValor = jQuery('#totValor').val();
			
			jQuery('#totiti').val(totValor);
			jQuery('#totitinerario').html(MascaraMonetario(totValor+''));

			var alim = jQuery('#auxalim').val();

			jQuery('#auxalimentacao').html(MascaraMonetario(alim+''));

			var transp = jQuery('#auxtransp').val();

			jQuery('#auxtransporte').html(MascaraMonetario(transp+''));

			var totdias = <?=$solicitacao['soltotdias']; ?>;

			var total = parseFloat(totValor)-(parseFloat(alim)+(parseFloat(transp)*parseFloat(totdias)));
			
			jQuery('#auxtot').val(total);
			jQuery('#auxtotal').html(MascaraMonetario(total+''));
			
		});
		
		</script>				
	</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr id="trcpf">
				<td class="SubTituloDireita" style="<?=$cpfShow ?>" width="12%">CPF</td>
				<td colspan="7">
					<?=$solicitacao['usucpf']?>
				</td>
				<td rowspan="9" width="30%" valign="bottom">
					<div id="deducao" style="margin-bottom:20px;">
						<?php 
							if($solicitacao['solcomplemento']){
								
								caixaComplemento( $_REQUEST['solid'] );
							}
						?>
						<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" width="100%" cellpadding="3" cellspacing="1">
							<tr>
								<td class="SubTituloCentro" colspan="2">Deduções</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Valor Itinerário</td>
								<td>
									R$ <label class="deducoes" id="totitinerario"></label>
									<input class="deducoes" type="hidden" id="totiti"/>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" width="50%">Dedução Alimentação</td>
								<td style="color:red">
									- R$ <label class="deducoes" id="auxalimentacao"></label>
									<input class="deducoes" type="hidden" id="auxalim" name="auxalim" value="<?=$solicitacao['soldeducaoalim']; ?>"/>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Dedução Transporte</td>
								<td style="color:red;border-bottom: 1px solid black;">
									- R$ <label class="deducoes" id="auxtransporte"></label>
									<input class="deducoes" type="hidden" id="auxtransp" name="auxtransp" value="<?=$solicitacao['soldeducaotransp']; ?>"/>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita"><b>Valor Total da Ajuda de Custo</b></td>
								<td>
									R$ <label class="deducoes" id="auxtotal"></label>
									<input class="deducoes" type="hidden" id="auxtot"/>
								</td>
							</tr>
						</table>
					</div>
				</td>
			</tr>
			<tr id="trnumdoc" style="<?=$numdocShow ?>">
				<td class="SubTituloDireita">N° do Documento:<br /> (Estrangeiro)</td>
				<td colspan="7">
					<?=$solicitacao['solnumdocestrangeiro']?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Nome</td>
				<td colspan="7">
					<?=$solicitacao['solnome']?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Email</td>
				<td colspan="7">
					<?=$solicitacao['solemail']?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Banco</td>
				<td colspan="7">
					<?=$solicitacao['solbanco']?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Agência</td>
				<td colspan="7">
					<?=$solicitacao['solagencia']?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Conta Corrente</td>
				<td colspan="7">
					<?=str_replace(Array('.','-'),Array('',''),$solicitacao['conta'])?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Código SCDP</td>
				<td colspan="7">
					<?=$solicitacao['solcodigoscdb']=='NULL'?'':$solicitacao['solcodigoscdb'];?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Observação</td>
				<td colspan="7">
					<?=$solicitacao['solobservacao']=='NULL'?'':$solicitacao['solobservacao'];?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Objetivo da Viagem</td>
				<td colspan="7">
					<?=$solicitacao['solobjetivo']=='NULL'?'':$solicitacao['solobjetivo'];?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="9">Itinerário</td>
			</tr>
			<tr>
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="movimentacao">
				<tr>
					<td class="SubTituloDireita" valign="bottom" style="text-align: center">Escala/Conexão</td>
					<td class="SubTituloDireita" valign="bottom" style="text-align: center">Origem</td>
					<td class="SubTituloDireita" valign="bottom" style="text-align: center">Destino</td>
					<td class="SubTituloDireita" valign="bottom" style="text-align: center">Permanência</td>
					<td class="SubTituloDireita" valign="bottom" style="text-align: center">Valor Diárias</td>
					<td class="SubTituloDireita" valign="bottom" style="text-align: center">Adicional por Embarque</td>
					<td class="SubTituloDireita" valign="bottom" style="text-align: center">Valor Total(R$)</td>
				</tr>
			<?php
			$totDias  = 0;
			$totPer   = 0;
			$totEmb   = 0;
			$totValor = 0;
//ver($viagem);
			foreach( $viagem as $itinerario ){ 

				$trecho++;
				$totDias  += $itinerario['itidias'];
				$totEmb   += $itinerario['emb'];
				$totValor += $itinerario['valor'];
				if($itinerario['itiultimo'] == 't'){$ultimodias=0.5;}else{$ultimodias=0;}
				
			?>
					<tr style="background-color: rgb(230,230,230)">
						<td align="center">
							<?=$itinerario['escala'] ?>
						</td>
						<td align="center">
							<?=$itinerario['origem'] ?> 
						</td>
						<td align="center">
							<?=$itinerario['destino'] ?>
						</td>
						<td align="center">
							<?=$itinerario['itidias'] ?> Dias <?php if($itinerario['itiultimo'] == 't'){?> <label style="color:red" title="Adicional ultimo trecho">(0.5)</label>  <?php } ?>
						</td>
						<td align="center">
							<label title="Valor da ajuda de custo<?=$itinerario['itimeiadiaria']=='t' ? '(meia)' : ''; ?>: R$ <?=str_replace('.',',',number_format(($itinerario['valordiaria']),2)) ?>" >R$ <?=str_replace('.',',',number_format($itinerario['valorviagem'],2)) ?></label>
						</td>
						<td align="center">
							R$ <?=str_replace('.',',',number_format($itinerario['emb'],2)) ?>
						</td>
						<td align="center">
							R$ <?=str_replace('.',',',number_format($itinerario['valor'],2)) ?>
						</td>
					</tr>
			<?php 
			}
			?>
					<tr id="bordainferior" style="background-color: rgb(215,215,215)">
						<td class="SubTituloDireita" valign="bottom" colspan="8"> </td> 
					</tr>
					<tr style="background-color: rgb(230,230,230)">
						<td class="SubTituloDireita" valign="bottom" colspan="3"> Total:</td> 
						<td align="center"> <?=$totDias ?> Dias</td>
						<td align="center"> - </td>
						<td align="center"> R$ <?=str_replace('.',',',number_format($totEmb,2)) ?></td>
						<td align="center"> R$ <?=str_replace('.',',',number_format($totValor,2)) ?><input type="hidden" id="totValor" value="<?=$totValor ?>" /></td>
					</tr>
				</table>
			</tr>
	</table>	
	</body>
</html>
