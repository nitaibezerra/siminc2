<?php

///echo "acao:".$_REQUEST['ctrid'];

$aciid = (integer) $_REQUEST['aciid'];

$sql = "
	select
		p.crtid
	from cte.acaoindicador a
		inner join cte.pontuacao p on
			p.ptoid = a.ptoid
	where
		a.aciid = " . $aciid . "
";
$crtid = (integer) $db->pegaUm( $sql );

$sqlProposta = "
	select
		psub.*,
		prg.prgid,
		prg.prgdsc
	from cte.proposicaosubacao psub
		left outer join cte.proposicaoacao pro on pro.ppaid = psub.ppaid
		left outer join cte.criterio crt on crt.crtid=pro.crtid
		inner join cte.programa prg on prg.prgid = psub.prgid
	where
		pro.crtid = " . $crtid . " and
		psub.ppsid not in (
			select
				coalesce( ppsid, 0 )
			from cte.subacaoindicador
			where
				aciid = " . $aciid . "
		)
	order by
		ppsordem
";

//dump( $sqlProposta );

$dadosProposta = $db->carregar( $sqlProposta );

?>
<html>
<head>
<title></title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
	
	function enviarProposta( ppsid, ppsdsc, undid, ppstexto, ppsobjetivo, prgid, ppsordem, ppsmetodologia, frmid, programa )
	{
		window.opener.propostaSubAcao( ppsid, ppsdsc, undid, ppstexto, ppsobjetivo, prgid, ppsordem, ppsmetodologia, frmid, programa );
		window.close();
	}
	
</script>
</head>
<body  LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff" style="background-image: url( '/imagens/fundo.gif' ); background-repeat: repeat-y;">

<table width="100%" align="center" border="0" class="tabela" cellpadding="1" cellspacing="1" >
	<tbody>
		<tr>
           <td style="padding:1px; background-color:#fafafa; color:#404040; vertical-align: top;" >
				<form method="POST" name="formulario2">
					
					<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td class="SubTituloEsquerda"><strong>Selecione:</strong></td>
							<td class="SubTituloEsquerda"><strong>Descrição Proposta de Sub Ação:</strong></td>
						</tr>
						<? if(!empty($dadosProposta)){ ?>
							<? for($i = 0; $i < count($dadosProposta);$i++ ){  ?>
								<?
								
								$dadosForma   = $db->carregar(" select fe.frmid,fe.frmdsc from cte.formaexecucao fe
															  where fe.frmid='".$dadosProposta[$i]['frmid']."' ");	
								$dadosUnidade = $db->carregar(" select um.undid,um.unddsc from cte.unidademedida um 
															  where um.undid='".$dadosProposta[$i]['undid']."' ");
								
								$ppsid	 		= $dadosProposta[$i]['ppsid'];
								$ppsdsc 		= $dadosProposta[$i]['ppsdsc'];
								$undid          = $dadosProposta[$i]['undid'];
								$ppstexto       = $dadosProposta[$i]['ppstexto'];
								$ppsobjetivo    = $dadosProposta[$i]['ppsobjetivo'];
								$prgid          = $dadosProposta[$i]['prgid'];
								$ppsordem       = $dadosProposta[$i]['ppsordem'];
								$ppsmetodologia = $dadosProposta[$i]['ppsmetodologia'];
								$frmid          = $dadosProposta[$i]['frmid'];
								
								//$subPrograma    = $dadosForma[0]['frmdsc'];
								
								$subForma 	    = $dadosForma[0]['frmdsc'];
								$subUnidade     = $dadosUnidade[0]['unddsc'];
								
								$programa = $dadosProposta[$i]['prgdsc'];
								
								?>
								
								<tr>
									<td class="TextoEsquerda">
										<INPUT
											TYPE="RADIO"
											$frmidNAME="OPCAO"
											VALUE="<?= $i; ?>"
											onclick="enviarProposta('<?= $ppsid ?>','<?= $ppsdsc ?>','<?= $undid ?>','<?= $ppstexto ?>','<?= $ppsobjetivo ?>','<?= $prgid ?>','<?= $ppsordem ?>','<?= $ppsmetodologia ?>','<?= $frmid ?>','<?= $programa ?>');"
										/>
									</td>
									<td class="TextoEsquerda">
										<strong>Nome:</strong> <?= $subNome; ?><br>
										<strong>Metodologia:</strong> <?=$subMetodologia; ?><br>
										<strong>Programa:</strong> <?= $subPrograma; ?><br>
										<strong>Forma:</strong> <?=  $subForma; ?><br>
										<strong>Unidade:</strong> <?= $subUnidade; ?><br>
									</td>
								</tr>
							<? } ?>
						<? } ?>
					</table>
				</form>	
			</td>
	     </tr>
	</tbody>
</table>

</body>
</html>