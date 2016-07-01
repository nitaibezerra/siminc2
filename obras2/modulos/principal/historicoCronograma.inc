<?php 
$crtid 						 = $_SESSION['obras2']['crtid'];
$obrid 						 = ($_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid']);
$_SESSION['obras2']['obrid'] = $obrid;

if ( $_POST['ajax'] == 'carregaCronograma' ){
	$obridContrato = $_POST['obridContrato']
?>
                <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                    <thead>
                        <tr>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Descrição</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Unidade de Medida</b></td>
                        </tr>
                    </thead>
<?php
                    $supervisaoItem = new SupervisaoItem();

//                    $total['vlrProjeto']           = 0;
//                    $total['vlrObra']              = 0;
//                    $total['vlrExecSobreObra']     = 0;
//                    $total['vlrExecSobreObraUlt']  = 0;
//                    $total['percExecSobreObraUlt'] = 0;

					// Busca filhos da Edificação
					$dadoEtapa = $supervisaoItem->getItensByEtapa($obridContrato, ($supid ? $supid : $ultimoSupid));
					$habil = (count($dadoEtapa) ? 'N' : 'S');
                    
					foreach ($dadoEtapa as $etapa){
						
//                        $total['vlrProjeto'] = $etapa['ocrvalorexecucao'];
//
//                        $total['vlrObra']  += $etapa['icovlritem'];
//                        $total['percObra']  = ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100;
//
//                        $total['vlrExecSobreObra']  += $etapa['spivlrfinanceiroinfsupervisor'];
//                        $total['percExecSobreObra']  = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;
//
//                        $total['vlrExecSobreObraUlt']  += $etapa['spivlrfinanceiroanterior'];
//                        $total['percExecSobreObraUlt']  = ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100;
//                        $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];
						
                        
						$etapa['icodtinicioitem'] = formata_data( $etapa['icodtinicioitem'] );
					    $etapa['icodterminoitem'] = formata_data( $etapa['icodterminoitem'] );
					
					    $etapa['spipercsobreobrainfsupervisor'] = ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100;
					    
					    // Busca Filhos da Etapa
					    $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obridContrato, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
					    $habil = (count($dadoDetalhe) ? 'N' : 'S');
?>
                            <tbody>
                                <tr bgcolor="#FDF8E7" id="<?="tr_item_etapa_" . $obridContrato . "_" . $etapa['icoid']?>">
                                    <td align="left">
                                    <input type="hidden" name="icoid[]" value="<?=$etapa['icoid']?>">
                                    <input type="hidden" name="ico_spiid[]" value="<?=$etapa['spiid'] ?>">
                                    &nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$etapa['itcdesc'] ?>
                                    </td>
                                    <td align="center">
                                    <?=$etapa['icodtinicioitem']  ?>
                                    </td>
                                    <td align="center">
                                    <?=$etapa['icodterminoitem']  ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    <?=number_format($etapa['icovlritem'], 2, ',', '.') ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    &nbsp;
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    &nbsp;
                                    </td>
                                </tr>
                            </tbody>
<?php
                            foreach ($dadoDetalhe as $detalhe){
                                $ditid = $detalhe['ditid'];
                                $detalhe['ditdatainicio']  = formata_data( $detalhe['ditdatainicio'] );
                                $detalhe['ditdatatermino'] = formata_data( $detalhe['ditdatatermino'] );
//                                $detalhe['spipercsobreobrainfsupervisor'] = ($detalhe['spivlrfinanceiroinfsupervisor'] / $detalhe['ocrvalorexecucao']) * 100;

                                //adicionando os filhos
                                $dadoDetalheFilho = $supervisaoItem->getItensByDetalhamento($obridContrato, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai ={$ditid}"));
                                $habil = (count($dadoDetalheFilho) ? 'N' : 'S');
?>
                                <tbody>
                                    <tr bgcolor="#DBF6D5" id="<?="tr_item_detalhamento_" . $obridContrato . "_" . $detalhe['icoid'] . "_" . $detalhe['ditid']?>">
                                        <td align="left">
                                        <input type="hidden" name="ditid[]" value="<?=$detalhe['ditid']?>">
                                        <input type="hidden" name="dit_spiid[]" value="<?=$detalhe['spiid'] ?>">
                                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$detalhe['ditdsc'] ?>
                                        </td>
                                        <td align="center">
                                        <?=$detalhe['ditdatainicio']  ?>
                                        </td>
                                        <td align="center">
                                        <?=$detalhe['ditdatatermino']  ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=number_format($detalhe['ditvalor'], 2, ',', '.') ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=($detalhe['ditmetafisica'] ? number_format($detalhe['ditmetafisica'], 2, ',', '.') : '-') ?>
                                        </td>
                                        <td align="center">
                                        <?=($detalhe['umcdsc'] ? $detalhe['umcdsc'] : '-') ?>
                                        </td>
                                    </tr>
                                </tbody>
<?php
                                foreach ($dadoDetalheFilho as $detalheFilho){
                                    $detalheFilho['ditdatainicio']  = formata_data( $detalheFilho['ditdatainicio'] );
                                    $detalheFilho['ditdatatermino'] = formata_data( $detalheFilho['ditdatatermino'] );

                                    $detalheFilho['spipercsobreobrainfsupervisor'] = ($detalheFilho['spivlrfinanceiroinfsupervisor'] / $detalheFilho['ocrvalorexecucao']) * 100;
?>
                                    <tbody>
                                        <tr bgcolor="#D7EAF2" id="<?="tr_item_detalhamento_filho_" . $obridContrato . "_" . $detalheFilho['icoid'] . "_" . $detalheFilho['ditidpai'] . "_" . $detalheFilho['ditid']?>">
                                            <td align="left">
                                            <input type="hidden" name="ditid_filho[]" value="<?=$detalheFilho['ditid']?>">
                                            <input type="hidden" name="dtf_spiid[]" value="<?=$detalheFilho['spiid'] ?>">
                                            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$detalheFilho['ditdsc'] ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=number_format($detalheFilho['ditvalor'], 2, ',', '.') ?>
                                            </td>
                                            <td align="center">
                                            <?=$detalheFilho['ditdatainicio']  ?>
                                            </td>
                                            <td align="center">
                                            <?=$detalheFilho['ditdatatermino']  ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=($detalheFilho['ditmetafisica'] ? number_format($detalheFilho['ditmetafisica'], 2, ',', '.') : '-') ?>
                                            </td>
                                            <td align="center">
                                            <?=($detalheFilho['umcdsc'] ? $detalheFilho['umcdsc'] : '-') ?>
                                            </td>
                                        </tr>
                                    </tbody>
<?php
                                }
                            }
                        }
?>
                </table>	
<?php 
	die;
}
?>	
<html>
    <head>
        <title>Histórico do Cronograma</title>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        <script type="text/javascript">
        function abreAditivo(crtid, obridContrato, expandir){
			if ( expandir == true ){
				$('#img_expandir_' + crtid).attr('src', '/imagens/menos.gif')
										   .attr('onclick', '')
										   .unbind('click')
										   .click(function () {abreAditivo(crtid, false)} );
				if ( $('#div_detalhe_cronograma_' + crtid).val() == '' ){
					$('#div_detalhe_cronograma_' + crtid).load("?modulo=principal/historicoCronograma&acao=A", {ajax: 'carregaCronograma', obridContrato: obridContrato});
				}
				$('#tr_detalhe_cronograma_' + crtid).show();
			}else{
				$('#tr_detalhe_cronograma_' + crtid).hide();
				$('#img_expandir_' + crtid).attr('src', '/imagens/mais.gif')
										   .attr('onclick', '')
										   .unbind('click')
										   .click(function (){ abreAditivo(crtid, true) });
			}
        
        }	
        </script>
    </head>
<body>
<?php
echo cabecalhoObra($obrid);
//echo '<br>';
monta_titulo( $titulo_modulo, '' );

$contrato   = new Contrato();
$arContrato = $contrato->listaHistoricoByContratoPai( $crtid );

$tipoTermo = new TipoTermoAditivo();

$obraContrato = new ObrasContrato();
?>
<table width="95%" cellspacing="1" cellpadding="2" border="0" align="center" class="listagem" id="vinculo_obra_modelo" style="background: #E9E9E9;">
	<tr>
		<th>
			Ação
		</th>
		<th>
			Nº		
		</th>
		<th>
			Aditivo		
		</th>
		<th>
			Tipo		
		</th>
	</tr>
<?php 
	foreach ($arContrato as $dadoContrato):
		if ( $dadoContrato['ttaid'] ){
			$i++;
			$dadoContrato['numero'] = $i;
			$tipoTermo->carregarPorId( $dadoContrato['ttaid'] );
			$dadoContrato['ttadsc'] = $tipoTermo->ttadsc; 
		}else{
			$dadoContrato['numero'] 		= '-';
			$dadoContrato['ttadsc'] 		= '-'; 
			$dadoContrato['crtdenominacao'] = '<b>Cronograma Original</b>'; 
		}
		$obridContrato = $obraContrato->pegaObridHistoricoByContratoAndObraPai( $dadoContrato['crtid'], $obrid );
?>	
	<tr>
		<td align="center">
			<img border="0" src="/imagens/mais.gif" id="img_expandir_<?php echo $dadoContrato['crtid']; ?>" onclick="abreAditivo(<?php echo $dadoContrato['crtid']; ?>, <?php echo $obridContrato; ?>, true)">
		</td>
		<td align="center">
			<?php echo $dadoContrato['numero']; ?>		
		</td>
		<td>
			<?php echo $dadoContrato['crtdenominacao']; ?>		
		</td>
		<td>
			<?php echo $dadoContrato['ttadsc']; ?>		
		</td>
	</tr>
	<tr style="display: none; background: #FFFFFF;" id="tr_detalhe_cronograma_<?php echo $dadoContrato['crtid']; ?>">
		<td align="center" colspan="4">
		<fieldset>
			<legend>Cronograma</legend>
			<div id="div_detalhe_cronograma_<?php echo $dadoContrato['crtid']; ?>"></div>
		</fieldset>
		</td>
	</tr>
<?php 
	endforeach;
?>	
</table>

</body>
</html>