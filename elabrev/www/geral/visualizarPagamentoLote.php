<?php

	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	$db = new cls_banco();
	
	if( $_REQUEST['requisicao'] == 'cancelarLote' ){
		
		$sql = "
            INSERT INTO elabrev.previsaoparcela(
                    proid, ppavlrparcela, codsigefnc, tcpnumtransfsiafi, ppadata, ppacancelarnc, ppamesenvio, ppanumeromacro, codncsiafi, ppanumcancelanc
            )
            SELECT
                proid, ppavlrparcela, codsigefnc, tcpnumtransfsiafi, ppadata, true, ppamesenvio, ppanumeromacro, codncsiafi, '{$_REQUEST['ppanumcancelanc']}'
            FROM elabrev.previsaoparcela WHERE ppaid = {$_REQUEST['ppaid']}
        ";
		
		$db->executar( $sql );
		if ($db->commit()) {
			echo '1';
		} else {
			echo '0';
		}
		exit;
	}
	
	if($_REQUEST['requisicao'] == 'carregaCancelamento'){
		
		if(!$_REQUEST['ppaid']) die;
		
		$sql = "
				SELECT DISTINCT
					pro.proid,
					ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
					substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
					substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
					pro.ptrid,
					a.acacod,
					pro.pliid,
					case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
					pro.ndpid,
					to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
					coalesce(pro.provalor, 0) as valor,
					crdmesliberacao,
					crdmesexecucao,
					pro.proid,
					pro.proanoreferencia,
					pro.prodata
				FROM monitora.previsaoorcamentaria pro
				LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
				LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
				LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
				LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
				LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
				LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
				LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
				WHERE pro.prostatus = 'A'
				AND pro.proid = (SELECT proid FROM elabrev.previsaoparcela WHERE ppaid = {$_REQUEST['ppaid']})";
		
		$rs = $db->pegaLinha($sql);
		
		echo '<center>
				<p></p>	  
				  <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
				  	<tr>
				  		<td class="subtitulocentro" colspan="2">Cancelar célula orçamentária</td>
				  	</tr>
				  	<tr>
				  		<td class="subtitulodireita">Número da NC de cancelamento</td>
				  		<td>
				  			<input type="text" name="ppanumcancelanc" id="ppanumcancelanc" value="" /> 
				  		</td>
				  	</tr>
				  	<tr>
				  		<td class="subtitulocentro" colspan="2">
				  			<input type="button" id="confirmarcancelamento" value="Enviar" />
				  			<input type="button" id="fechar" value="Fechar" />
				  			<input type="hidden" id="ppaid" name="ppaid" value="'.$_REQUEST['ppaid'].'" />		
				  		</td>
				  	</tr>
				  </table>
				 </center>
				';
		
		echo '
				<p></p>
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<tr>
						<td colspan="2" class="subtitulocentro">Detalhe</td>
					</tr>
				  	<tr>
				  		<td class="subtitulodireita">Ano</td>
						<td>'.$rs['proanoreferencia'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Ação</td>
						<td>'.$rs['acacod'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Programa de Trabalho</td>
						<td>'.$rs['ptrid_descricao'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Plano Interno</td>
						<td>'.$rs['pliid_descricao'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Descrição da Ação Constante da LOA</td>
						<td>'.$rs['acatitulo'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Nat.da Despesa</td>
						<td>'.$rs['ndp_descricao'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Valor (em R$ 1,00)</td>
						<td>'.$rs['provalor'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Mês da Liberação</td>
						<td>'.$rs['crdmesliberacao'].'</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Prazo para o cumprimento do objeto</td>
						<td>'.$rs['crdmesexecucao'].'</td>
				  	</tr>
				</table>
		
				';
		exit;
	}

	$titulo = "Visualziação de Lote";

	$sql = "
		select
		    proid,
			ppaid,
			ppamesenvio,
			tcpnumtransfsiafi,
			case when ppanumcancelanc is not null then ppanumcancelanc else codncsiafi end as codncsiafi,
			ppavlrparcela,
			ppacancelarnc
		from elabrev.previsaoparcela
		where
			--ppacancelarnc = false and 
			 codncsiafi = '{$_GET['codncsiafi']}'
	";
	
	$listaParcelas = $db->carregar($sql);
?>
<html>
	<head>

		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
		<title><?= $titulo ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
		

		<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../../includes/jquery-ui/jquery-ui-1.8.20.custom.min.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/jquery-ui/jquery-ui-1.8.22.custom.css"/>
		<script>
			jQuery.noConflict();
			jQuery(document).ready(function(){

				jQuery('.cancelarCelula').click(function(){

					jQuery("#dialog").html('');				

					var tr = jQuery(this).parent().parent(); 

					if(confirm('Deseja cancelar este valor?')){

						jQuery.ajax({
							url		: document.location,
							type	: 'post',
							data	: 'requisicao=carregaCancelamento&ppaid='+jQuery(this).attr('codigo'),
							success	: function(e) {
								jQuery("#dialog").html(e);		
							}
						});

						jQuery("#ppaid").val(jQuery(this).attr('codigo'));
						jQuery("#dialog").dialog( "open" );
						
					}
					
				});

				jQuery('#confirmarcancelamento').live('click', function(){

					if(jQuery('#ppanumcancelanc').val() == ''){
						alert('O campo "Número da NC de cancelamento" é obrigatório!');
						jQuery('#ppanumcancelanc').focus();
						return false;
					}
					
					jQuery.ajax({
						url		: 'visualizarPagamentoLote.php',
						type	: 'post',
						data	: 'requisicao=cancelarLote&ppaid='+jQuery('#ppaid').val()+'&ppanumcancelanc='+jQuery('#ppanumcancelanc').val(),
						success	: function(e){								
							if(e == '1'){								
								alert('Registro excluido com sucesso.');
								window.opener.location.reload()
							}else{
								alert('Não foi possível excluir o registro!');
							}
						}
					});
				});

				jQuery("#dialog").dialog({
				      autoOpen: false,
				      show: {
				        effect: "blind",
				        duration: 1000
				      },
				      hide: {
				        effect: "explode",
				        duration: 1000
				      }
			    });
			    
				jQuery(".ui-dialog-titlebar").hide();

				jQuery('#fechar').live('click', function(){
					jQuery("#dialog").dialog( "close" );
				});
			});
		</script>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">

		<form name="formularioEnvioPagamento" method="post" action="">
		<input type="hidden" name="requisicao" value=""/>
		<input type="hidden" name="codncsiafi" value="<?=$_GET['codncsiafi']?>"/>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" valign="bottom" colspan="4">
					<center><b>Informações complementares</b></center>
				</td>
			</tr>
			<tr>
				<td colspan='3'>
					<table>
					<tr>
						<td class="SubTituloDireita">Número de Transferência (SIAF):</td>
						<td colspan="2"><?=$listaParcelas[0]['tcpnumtransfsiafi']?></td>
					</tr>
					</table>
				</td>
			</tr>
			<tr>
				<!--<td class="subtitulocentro"><b>Ações</b></td>-->
				<td class="subtitulocentro"><b>Mês de Liberação</b></td>
				<td class="subtitulocentro"><b>NC</b></td>
				<td class="subtitulodireita"><b>Valor</b></td>
			</tr>
			<?php 
			$total = 0;
			foreach ($listaParcelas as $key => $value) { ?>
				<tr>
                    <!--
					<td style="text-align:center;">
						<?php //if($value['ppacancelarnc'] == 'f'): ?>
							<img style="cursor:pointer;" src="../../imagens/exclui_p.gif" codigo="<?php echo $value['ppaid']; ?>" class="cancelarCelula" alt="Cancelar celula orçamentária" title="Cancelar celula orçamentária" />
						<?php //endif; ?>
					</td>
					-->
					<td style="text-align:center;"><?=mes_extenso($value['ppamesenvio'])?></td>
					<td style="text-align:center;"><?=$value['codncsiafi']?></td>
					<td style="text-align:right;"><?php if($value['ppacancelarnc'] == 't') echo "-<b>" ?><?=number_format($value['ppavlrparcela'],2,',','.')?></b></td>
				</tr>
				<?php

                if ($value['ppacancelarnc'] == 't') {
                    $total = $total-$value['ppavlrparcela'];
                } else {
                    $total = $total+$value['ppavlrparcela'];
                }

                $remanejamento = $db->pegaLinha("select * from elabrev.creditoremanejado where proid = {$value['proid']}");
                if (is_array($remanejamento)) { ?>
                    <tr style="color:red;">
                        <!--<td style="text-align:center;">&nbsp;</td>-->
                        <td style="text-align:center;">&nbsp;</td>
                        <td style="text-align:center;"><?= str_replace(array('nc', 'NC'), '', $remanejamento['nc_devolucao']); ?></td>
                        <td style="text-align:right;"><string>- <?= number_format($remanejamento['valor'], 2, ',', '.'); ?></string></td>
                    </tr>
                    <?php
                }

                if (is_array($remanejamento)) {
                    $total = $total-$remanejamento['valor'];
                }
			}
			?>
			<tr>
				<td class="subtituloesquerda" colspan="2">Total:</td>
				<td class="subtitulodireita"><b><?=number_format($total,2,',','.')?></b></td>
			</tr>
			
			<!--
			<tr>
				<td style="width:15%;text-align:right;"></td>
				<td style="text-align:left;"><input type="button" onclick="javascript:cancelarLote()" value="Cancelar Lote"/></td>
			</tr>
			-->
			
		</table>
		</form>

	</body>

	<!--  
	<script>
		function cancelarLote(){
			if( confirm("Deseja realmente cancelar esse lote?") ){
				$('[name="requisicao"]').val('cancelarLote');
				$('[name="formularioEnvioPagamento"]').submit();
			}
		}
	</script>
	-->
	
	<div id="dialog" title="Basic dialog">
	  
	  
	</div>
</html>
