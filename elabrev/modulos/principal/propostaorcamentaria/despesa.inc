<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral, Gilberto Arruda Cerqueira Xavier
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:despesa.inc
 
   */

// verifica, caso tenha escolhido a ação, se o usuário tem permissão pra manipular a unidade (indicada na ação)
//if ( !$db->usuarioPossuiPermissaoUnidade( '26101' ) ) {
//	$db->sucesso( 'A Proposta Orçamentária 2012 ainda não está disponível para detalhamento. Por favor aguarde...' );
//}

/* Bloqueio momentaneo para a PLOA 2013/2014 */
//$arrUnidades = array('26291','26290', '26291','26298', '26443');
//
//if( !usuarioPossuiPermissaoUnidades($arrUnidades) ){
//	$db->insucesso('Prazo para detalhamento da Proposta 2014 encerrado.' );
//}
/* FIM  Bloqueio momentaneo para a PLOA 2013/2014 */

$sql = "select ppoid from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoano = '" . ( $_SESSION['exercicio'] + 1 ) . "'";
$id_proposta_ativa = $db->pegaUm( $sql );

//Recupera tudo que foi passado por REQUEST e instancia as variáveis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;
//Agrupamento da listagem
if (!$ordemgrupo) $ordemgrupo='A';
if ($ordemgrupo=='L') $agrupadopor = 'Localizador'; elseif ($ordemgrupo=='U') $agrupadopor = 'Unidade'; else $agrupadopor = 'Ação';
//Ordem da listagem
if (!$ordemlista) $ordemlista = 2;
//Direcionamento da ordenação ASC / DESC
if ($ordemlistadir <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else $ordemlistadir2 = 'ASC';
//Controla se vai aparecer os programas baseado na ação ou no parâmetro
if (!$listaprg and $acao=='A') $listaprg='N'; elseif (!$listaprg) $listaprg='N';
/*
if ( $db->usuarioPossuiPermissaoTodasUnidades() )
{
	$join_responsabilidade = "";
}
else
{
	$join_responsabilidade = " inner join elabrev.usuarioresponsabilidade r on r.unicod = u.unicod and r.rpustatus = 'A' and r.usucpf = '".$_SESSION['usucpf']."' inner join seguranca.perfil p on p.pflcod = r.pflcod and p.sisid = ".$_SESSION['sisid'];
}
*/
/*
$unidades_validas = implode( "','", $db->usuarioUnidadesPermitidas() );
$unidades_validas = strlen( $unidades_validas ) > 0 ? "'" . $unidades_validas . "'" : 'null' ;
$join_responsabilidade = " inner join unidade on unidade.unicod in ( " . $unidades_validas . " )  ";
*/
$join_responsabilidade = $db->usuarioJoinUnidadesPermitidas();
$join_responsabilidade .= " and a.unicod = unijoin.unicod ";
//Verifica perfil para visualizar mensagem 
$arPerfil = array( 53, 54, 57, 55, 76, 388 );

$mostraShowModal = false;
if($db->testa_superuser() || possuiperfil($arPerfil)){
	//$mostraShowModal = true;
}


//Se não tiver escolhido nehum tipo traz todas
if (!$tipoacao1 and !$tipoacao2 and !$tipoacao3) {$tipoacao1='N';$tipoacao2='R';$tipoacao3='E';}
//filtros de tipo de ação
$wh .= " and a.acasnrap='f' ";

//Cabeçalho do sistema
include  APPRAIZ."includes/cabecalho.inc";

?>
<br>
<?

if ($listaprg=='N') {
		if ($ordemgrupo=='A') {
			$subtit1 = 'de Unidades';
			$subtit2 = 'da Unidade';
//			$sql= "select a.unicod, a.unicod as codigo, u.unidsc as descricao, a.prgano, count(*) as numacoes from elabrev.ppaacao_orcamento a inner join unidade u on a.unicod = u.unicod ".$join_responsabilidade." where prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' ".$wh." group by a.unicod, u.unidsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
			$sql =
				" select 
					 a.unicod, 
					 a.unicod as codigo,
					 u.unidsc as descricao, 
					 a.prgano, 
					 count(*) as numacoes, 
					 coalesce( b.total, 0 ) as valor, 
					 coalesce( c.despesa, 0 ) as despesa, 
					 coalesce( c.saldo, 0 ) as saldo 
				 from elabrev.ppaacao_orcamento a 
					 inner join unidade u on a.unicod = u.unicod
					 " . $join_responsabilidade . "
					 left join ( 
						 select 
							 u.unicod, 
							 sum( coalesce( l.lmfvalor, 0 ) ) as total 
						 from elabrev.limitefonte_calc2 l  
							 inner join unidade u ON l.unicod = u.unicod and l.unitpocod = u.unitpocod 
							 inner join elabrev.montantematriz mm using ( mtmid ) 
							 inner join elabrev.propostaorcamento po on 
									mm.ppoid=po.ppoid and 
									po.ppoanoexercicio = '" . $_SESSION['exercicio'] . "' and 
									po.ppoano = '" . ( $_SESSION['exercicio'] + 1 ) . "' and 
									po.ppostatus='A' and
									po.ppoid = l.ppoid 
							 inner join elabrev.matriz m using ( mtrid )
						 group by u.unicod 
						 ) as b ON b.unicod = u.unicod 
					 left join ( 
						 select 
							 su.unicod, 
							 sum( coalesce( vldespesa, 0 ) ) as despesa, 
							 sum( coalesce( saldo, 0 ) ) as saldo 
						 from elabrev.v_saldounidadefonte2 su 
						 where  su.ppoanoexercicio = '" . $_SESSION['exercicio'] . "' 
						 group by su.unicod, su.unidsc 
						 ) as c on c.unicod = a.unicod 
				 where a.acastatus='A' and a.prgano = '" . $_SESSION['exercicio'] . "' " . $wh . "
				 group by a.unicod, a.unicod , u.unidsc, a.prgano, b.total, c.despesa, c.saldo 
				 order by " . $ordemlista . " ". $ordemlistadir;
			}
		elseif ($ordemgrupo=='U') {
			$subtit1 = 'de Unidades'; 
			$subtit2 = 'da Unidade'; 
			$sql= "select a.unicod, a.unicod as codigo, u.unidsc as descricao, a.prgano, count(*) as numacoes from elabrev.ppaacao_orcamento a inner join unidade u on a.unicod = u.unicod ".$join_responsabilidade." where prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' ".$wh." group by a.unicod, u.unidsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
			}
		elseif ($ordemgrupo=='L') {
			$subtit1 = 'de Localizadores'; 
			$subtit2 = 'do Localizador';
			$sql = "select a.regcod, a.regcod as codigo, r.regdsc as descricao, a.prgano, count(*) as numacoes from elabrev.ppaacao_orcamento a left join regiao r on a.regcod=r.regcod where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."'  ".$wh." group by a.regcod, a.regcod , r.regdsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
			}
	}
else
{
	if ( $ordemlista > 4 )
	{
		$ordemlista = 2;
	}
	
	$tipoprgaca = "Programas";
	$sql =
		" select 
			 ppa.prgid, ppa.prgcod as codigo, 
			 ppa.prgdsc as descricao, a.prgano, 
			 count( distinct a.acaid ) as numacoes 
		 from elabrev.ppaacao_orcamento a 
			 inner join elabrev.ppaprograma_orcamento ppa on ppa.prgid = a.prgid 
			 inner join unidade u on a.unicod = u.unicod 
			  " . $join_responsabilidade ."		
			  left join elabrev.despesaacao c on c.acaid = a.acaid and c.ppoid = " . ( $id_proposta_ativa ? $id_proposta_ativa : 'null' ) . " 
		 where 
			 a.acastatus = 'A' and 
			 a.prgano = '" . $_SESSION['exercicio'] . "' and 
			 a.acasnrap = false 
		 group by ppa.prgid, ppa.prgcod, ppa.prgdsc, a.prgano 
		 order by " . $ordemlista . " ". $ordemlistadir;
	$subtit1 = 'de Programas'; 
	$subtit2 = 'do Programa';
}
// dbg($listaprg);
// dbg($ordemgrupo);
// dbg($sql);
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,'Para detalhar as despesas selecione uma Ação na árvore abaixo - Total '.$subtit1.' ('.($nlinhas+1).')<br>Para ver os Dados Orçamentarios, clique no código da Unidade.');?>



<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
    <form name="formulario" method="post">
	<input type="Hidden" name="ordemlista" value="<?=$ordemlista?>">
	<input type="Hidden" name="ordemlistadir" value="<?=$ordemlistadir?>">
	<tr>
	<td class="SubTituloDireita">Mostrar:</td><td style="color:#008000;"><input onclick="filtra();" type="radio" name="listaprg" value="S"  <?if ($listaprg=='S') print 'checked';?>> Programas e Ações <input onclick="filtra();" type="radio" name="listaprg" value="N"  <?if ($listaprg=='N') print 'checked';?>> Apenas Ações  </td>
	<!--
	<td class="SubTituloDireita" style="text-align:center;"><input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="filtra();"></td>
	-->
	</tr>
	</form>
</table>

<? if ($nlinhas >= 0) {?>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
<? if ($listaprg == 'S') { ?>
      <td style="width:65px;" nowrap class="title"><?if ($ordemlista == '2') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('2','<?=$ordemlistadirnova?>');" title="Ordenar por Código"><strong>Código</strong></label></td>
      <td width="100%" class="title"><?if ($ordemlista == '3') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('3','<?=$ordemlistadirnova?>');" title="Ordenar por Título <?=$subtit2?>"><strong>Título <?=$subtit2?></strong></label></td>
      <td style="width:100px;" nowrap class="title" align="right"><?if ($ordemlista == '5') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('5','<?=$ordemlistadirnova?>');"  title="Ordenar por Total"><strong>Localizadores</strong></label></td>
<? }
	else 
	{ ?>

      <td style="width:65px;" nowrap class="title"><?if ($ordemlista == '2') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('2','<?=$ordemlistadirnova?>');" title="Ordenar por Código"><strong>Código</strong></label></td>
	  <td width="100%" class="title"><?if ($ordemlista == '3') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('3','<?=$ordemlistadirnova?>');" title="Ordenar por Título <?=$subtit2?>"><strong>Título <?=$subtit2?></strong></label></td>
	  <td style="width:100px;" class="title" align="right" nowrap><?if ($ordemlista == '6') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('6','<?=$ordemlistadirnova?>');"  title="Ordenar por Limite"><strong>Limite</strong></label></td>
	  <td style="width:100px;" class="title" align="right" nowrap><?if ($ordemlista == '7') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('7','<?=$ordemlistadirnova?>');"  title="Ordenar por Limite"><strong>Despesa</strong></label></td>
	  <td style="width:100px;" class="title" nowrap align="right"><?if ($ordemlista == '8') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('8','<?=$ordemlistadirnova?>');"  title="Ordenar por Total"><strong>Saldo</strong></label></td>
<?	} ?>
    </tr>
</thead>
<tbody>
<?
$totalacoes = 0;
$valor_total = 0;
$despesa_total = 0;
$saldo_total = 0;
for ( $i = 0; $i <= $nlinhas; $i++ ) {
	$res = $db->carrega_registro($RS,$i);
	// a linha abaixo transforma em variáveis todos os campos do array
	if( is_array( $res ) )
	{
		foreach($res as $k=>$v) ${$k}=$v;
	}
	if ( $nlinhas == 1 && $i == 1 )
	{
		$_SESSION['unicod_visualizacao'] = $unicod;
	}
	$valor_total += $valor;
	$despesa_total += $despesa;
	$saldo_total += $saldo;
	$totalacoes = $totalacoes + $numacoes;
	$cor = '';
	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
	?>
	<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
	<? if ( $listaprg == 'S' ) { ?>
		<td><?= $codigo; ?></td>
		<?if ( $descricao == '' ) $descricao = '<font color="red">Não atribuído pelo SIGPLAN</font>'; ?>
		<td onclick="abreconteudo(<? if ( $listaprg== 'S' ) { ?>'geral/listaacaodespesa.php?prgid=<?=$prgid?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?} else {?>'geral/listaacaodespesa.php?codigo=<?if ($ordemgrupo=='A') print $codigo; else print $codigo;?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?}?>,'<?=$i?>-<?=$codigo?>')" <?if ($listaprg=='N') print 'style="color:#003c7b"';?>> <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>"> <?=$descricao?></td>
		<td align=right>(<?=$numacoes?>)</td>
	<? } else { ?>
		<td valign="top" align="right">
			<a href="elabrev.php?modulo=principal/unidade&acao=A&unicod=<?= $codigo; ?>"><?= $codigo; ?><a/>
		</td>
		<td onclick="abreconteudo(<?if ($listaprg=='S') {?>'geral/listaacaodespesa.php?prgid=<?=$prgid?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?} else {?>'geral/listaacaodespesa.php?codigo=<?if ($ordemgrupo=='A') print $codigo; else print $codigo;?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?}?>,'<?=$i?>-<?=$codigo?>')" <?if ($listaprg=='N') print 'style="color:#003c7b"';?>> <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>"> <?=$descricao?></td>
		<td align="right" ><?= number_format($valor, 0, ',', '.') ?></td>
		<td align="right" ><?= number_format($despesa, 0, ',', '.') ?></td>
		<?
			if ($saldo < 0) $cor = 'red';
		?>
		<td align="right" ><font color="<?=$cor?>"><?= number_format($saldo, 0, ',', '.') ?></font></td>
	<? } ?>
	</tr>
	<tr bgcolor="<?=$marcado?>">
		<td id="td<?=$i?>-<?=$codigo?>" colspan="<? if ($listaprg == 'S') { print '3'; } else { print '5'; }?>" style="padding-left:65px;"></td>
	</tr>
<? } ?>
<? $marcado = fmod( $i, 2 ) == 0 ? '' : '#F7F7F7'; ?>
<? if ( $listaprg == 'N' ) : ?>
	<tr bgcolor="<?=$marcado?>">
		<td colspan="2" style="border-bottom: 1px solid #dedede;" align="right">
			Total Geral:
		</td>
		<td align="right" style="border-bottom: 1px solid #dedede;">
			<b><?= number_format( $valor_total, 0, ',', '.' ) ?></b>
		</td>
		<td align="right" style="border-bottom: 1px solid #dedede;">
			<b><?= number_format( $despesa_total, 0, ',', '.' ) ?></b>
		</td>
		<td align="right" style="border-bottom: 1px solid #dedede;">
			<b><?= number_format( $saldo_total, 0, ',', '.' ) ?></b>
		</td>
	</tr>
<? endif; ?>
<? $marcado = fmod( $i, 2 ) == 0 ? '' : 'marcado'; ?>
    <tr class="<?=$marcado?>">
	  <td></td>
      <td valign="top" class="title" colspan="<?if ($listaprg=='N') print '3'; ?>"> 
		  <table width="100%">
		  	<tr>
		  		<td style="border:none;"><strong>Total <?=$subtit1?>: <?=$i?></strong></td>
		  		<td align="right" style="border:none;"><strong>Total de Localizadores de Gasto:</strong></td>
		  	</tr>
		  </table>
	  </td>
	  <td align="right" ><strong><?=$totalacoes?></strong></td>
   </tr>
</tbody>
</table>
<? } else { ?>
<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
	<tr>
	<td align="center">Não foram encontrados registros</td>
	</tr>
</table>
<? }?>
	</td>
    </tr>
  </table>
  </center>
</div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript">

function filtra()
{
document.formulario.submit();
}

function ordena(ordem, direcao)
{
document.formulario.ordemlista.value=ordem;
document.formulario.ordemlistadir.value=direcao;
document.formulario.submit();
}

/*** INICIO SHOW MODAL ***/

var countModal = 1;

function montaShowModal() {
//	var campoTextArea = ''+
//		'<p align=\'justify\'>Informamos que esses dados correspondem à Proposta Orçamentária de 2011 encaminhada por essa Unidade ao MEC/SPO.'+
//		'<br /><br />Para visualizar a Proposta Orçamentária encaminhada ao Congresso Nacional pelo poder Executivo, o acesso deverá ser feito pelo endereço eletrônico: <a href=\'#\' onclick=\'redirecionaPDF(); closeMessage();\'>www.planejamento.gov.br <http://www.planejamento.gov.br> </a>'+
//		'por meio do link <a href=\'#\' onclick=\'redirecionaOrcamento(); closeMessage();\'><i>orçamentos anuais</i></a>.</p>';
/*
	var campoTextArea = ''+
		'<div>'+

		'<p align=center style=\'text-align:center\'><b style=\'mso-bidi-font-weight:normal\'>ELABORAÇÃO DO PLOA-2012</b></p>'+

		'<p align=\'justify\'>&nbsp;&nbsp;&nbsp;Informamos que os limites para o PLOA-2012 já se encontram disponíveis no SIMEC para que as unidades procedam os detalhamentos dos <b><u>valores</u></b> por programa e ação, bem como informem os <b><u><span style=\'color:blue\'>quantitativos das metas físicas</span></u></b> associadas a cada ação orçamentária, observando os detalhamentos das despesas por: <b><u>ação</u></b>, <b><u>subação</u></b>, <b><u>natureza</u></b>, <b><u>iduso</u></b>, <b><u>fonte</u></b> e <b><u>idoc,</u></b> de acordo com as respectivas matrizes de distribuição de recursos. As despesas de pessoal e benefícios a servidores não devem ser detalhadas pelas unidades, ficando essas a cargo da CGO/SPO/MEC.'+

		'<br /><br />&nbsp;&nbsp;&nbsp;O PLOA-2012 está sendo elaborado de acordo com novo PPA-2012/2015 e, para tanto, deve-se observar algumas alterações na forma de distribuição dos recursos nas unidades orçamentárias. Face a aglutinação de diversas ações orçamentárias para o próximo exercício, esta SPO disponibilizou no SIMEC, a possibilidade da utilização de subações para a distribuição dos recursos orçamentários no PLOA-2012, contribuindo para um melhor gerenciamento dos tipos de despesas a serem realizados em cada ação orçamentária.'+

		'<br /><br />&nbsp;&nbsp;&nbsp;Em consonância com o disposto no inciso XIII do Anexo I, do Projeto de Lei de Diretrizes Orçamentárias para 2012 – PLDO-2012, as despesas com <b><u>tecnologia da informação (TI)</u></b>, <b><u>diárias e passagens</u></b>, <b><u>locação de imóveis</u></b>, <b><u>aquisição de imóveis, reforma de bens imóveis</u></b>, <b><u>aquisição de veículos</u></b>, <b><u>locação de veículos</u></b> e <b><u>locação de máquinas e equipamentos</u></b>, <b><u><span style=\'color:blue\'>deverão ser informadas até o nível de subelemento</span></u></b> das naturezas</span>. <a href=\'#\' onclick=\'redirecionaPDF(); closeMessage();\'>Clique aqui </a>para visualizar a lista de naturezas de despesa.'+

		'<p align=\'justify\'>&nbsp;&nbsp;&nbsp;<b><u><span lang=IT style=\'color:blue\'>O prazo para encerrar o detalhamento do PLOA-2012 no SIMEC é 26/07/2011.</span></u></b></p>'+

		'</div>'
*/
	var campoTextArea = ''+
		'<div>'+

		'<p align=center style=\'text-align:center\'><b style=\'mso-bidi-font-weight:normal\'>Prazo para o detalhamento do PLOA-2012 encerrado</b></p>'+
		'</div>'

	var alertaDisplay = '<div class="titulo_box"><center><b>ATENÇÃO</b></center><br/>'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	messageObj.setHtmlContent(messageContent);
//	messageObj.setSize(700,420);
	messageObj.setSize(350,140);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

function redirecionaPDF(){
	windowOpen('http://www.planejamento.gov.br');
}
function redirecionaOrcamento(){
	windowOpen('http://www.planejamento.gov.br/secretaria.asp?cat=50&sec=8');
}

function verificaSeMostraShowModal(){
	var mostraShowModal = '<?php echo $mostraShowModal; ?>';
	if(mostraShowModal){
		montaShowModal();	
	}
}


$(document).ready(function(){
	verificaSeMostraShowModal();
});


function redirecionaPDF(){
	windowOpen('http://simec.mec.gov.br/anexo_subelemento.xls','','height = 300, width = 400, scrollbars = yes');
}


</script>