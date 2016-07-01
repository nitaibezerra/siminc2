<?php
if($_REQUEST['lbrid'])
	$_SESSION['elabrev_var']['lbrid'] = $_REQUEST['lbrid'];
	
if(!$_SESSION['elabrev_var']['lbrid']) {
	
	echo "<script>
			alert('Problemas nas variaveis enviadas');
			window.location='elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';
		  </script>";
	exit;
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );

$sql = "SELECT * FROM elabrev.liberacao WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
$liberacao = $db->pegaLinha($sql);

$abas = menuAbasLiberacao();

$sql = "SELECT 
		  	libnumeroliberacao||'/'||to_char(libdataliberacao, 'YYYY') as liberacao,
		  	libnumprocesso,
		  	libdescricao,
		  	libvalortotalprevisto,
			libdataprevinicio,
			tcdid,
			tpcid,
			libdscoutras,
			lbrnulibano
		FROM 
		  	elabrev.liberacao
		WHERE
			lbrstatus = 'A'
			and lbrid = ".$_SESSION['elabrev_var']['lbrid'];

$arRegistro = $db->pegaLinha( $sql );
$arRegistro = $arRegistro ? $arRegistro : array();
extract($arRegistro);

/*
 * REGRA, SE A LIBERAÇÃO ESTIVER EM ELABORAÇÃO, NÃO DEVERÁ MOSTRAR AS ABAS
 */


echo montarAbasArray($abas, $_SERVER['REQUEST_URI']);

cabecalhoLiberacao($_SESSION['elabrev_var']['lbrid']);


$libvalortotalprevisto = $libvalortotalprevisto ? number_format($libvalortotalprevisto, '2', ',', '.') : ''; 
$libdataprevinicio = $libdataprevinicio ? formata_data($libdataprevinicio) : '';

$anoAtual = $_SESSION['exercicio'];
$anoProximo = $anoAtual + 1;

$arExercicio = $db->carregar("SELECT lieexercicio, lievalorexercicio FROM elabrev.liberacaoexercicio WHERE lbrid = ".$_SESSION['elabrev_var']['lbrid']." order by lieexercicio asc");
$arExercicio = $arExercicio ? $arExercicio : array();

if( $arExercicio ){
	$anoAtual 				= $arExercicio[0]['lieexercicio'];
	$libvalorexercicioatual = number_format($arExercicio[0]['lievalorexercicio'], '2', ',', '.');
	
	$anoProximo 			  = $arExercicio[1]['lieexercicio'];
	$libvalorexercicioproximo = number_format($arExercicio[1]['lievalorexercicio'], '2', ',', '.');
}


?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="JavaScript" src="../includes/prototype.js"></script>


<form method='post' name='formulario' id="formulario">
<input type='hidden' name='requisicao' value='gravarDadosGeraisCDO'>

			
<div id="abas" >
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Dados Gerais</li>
			</ul>
		</div>
		<!-- Dados Gerais -->
		<div id="aba_dadosgerais" class="conteudoAba">
		
			<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td style="width: 95%">
				
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloDireita">Número do Processo:</td>
							<td><?=campo_texto('libnumprocesso', 'N', 'S', 'Número do Processo', 30, 20, '', '','','','','id="libnumprocesso"'); ?></td>
						</tr>
						<tr>
							<td class="subtitulodireita" valign=top>Assunto:</td>
							<td><?=campo_textarea('libdescricao', 'N', 'S', 'Assunto', 60, 3, 500, '','','','','Assunto',$libdescricao);?></td>
						</tr>
						<tr>
							<td class="subtitulodireita">Valor total previsto:</td>
							<td><?=campo_texto('libvalortotalprevisto', 'N', 'S', 'Valor total previsto', 15, 13, '[###.]###,##', '','','','','id="libvalortotalprevisto"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value); valorExercicioProximo();"); ?></td>
						</tr>
						<tr>
							<td class="subtitulodireita">Valor no exercício de <?=$anoAtual; ?>:</td>
							<td><?=campo_texto('libvalorexercicioatual', 'N', 'S', 'Valor no exercício de '.$anoAtual, 15, 13, '[###.]###,##', '','','','','id="libvalorexercicioatual"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value); valorExercicioProximo();"); ?>
								<input type='hidden' name='anoatual' value='<?=$anoAtual; ?>'>
							</td>
						</tr>
						<tr>
							<td class="subtitulodireita">Valor no exercício de <?=$anoProximo; ?>:</td>
							<td><?=campo_texto('libvalorexercicioproximo', 'N', 'N', 'Valor no exercício de '.$anoProximo, 15, 13, '[###.]###,##', '','','','','id="libvalorexercicioproximo"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value);"); ?>
								<input type='hidden' name='anoproximo' value='<?=$anoProximo; ?>'>
							</td>
						</tr>
						<tr>
							<td class="subtitulodireita">Data prevista de Início:</td>
							<td><?=campo_data2('libdataprevinicio', 'N','S','Data prevista de Início','','',''); ?></td>
						</tr>
						<tr>
							<td class="subtitulodireita">Tipo de CDO:</td>
							<td><?		
								$sql = "SELECT tcdid as codigo, tcddsc as descricao from elabrev.tipocdo where tcdstatus = 'A'";
								echo $db->monta_combo("tcdid", $sql, 'S','-- Selecione --','mostraUgs', '', '',150,'N','tcdid', '', '', 'Tipo de CDO');?></td>
						</tr>
						<tr id="tr_nulib" style="display: <?if($tcdid == 1 || !$tcdid) echo 'none';?>;">
							<td class="subtitulodireita">Nº Liberação:</td>
							<td><?		
								$sql = "SELECT li.libnumeroliberacao||'/'||to_char(li.libdataliberacao, 'YYYY') as codigo, li.libnumeroliberacao||'/'||to_char(li.libdataliberacao, 'YYYY') as descricao from elabrev.liberacao li where li.lbrstatus = 'A' order by li.lbrid";
								echo $db->monta_combo("lbrnulibano", $sql, 'S','-- Selecione --','', '', '',150,'N','lbrnulibano', '', '', 'Nº Liberação');?></td>
						</tr>
						<tr>
							<td class="subtitulodireita">Modalidade de Contratação:</td>
							<td><?		
								$sql = "SELECT tpcid as codigo, tpcdsc as descricao from elabrev.tipocontratacao where tpcstatus = 'A' order by 2";
								echo $db->monta_combo("tpcid", $sql, 'S','-- Selecione --','mostraContratacao', '', '',150,'N','tpcid', '', '', 'Modalidade de Contratação');
								?>
							</td>
						</tr>
						<tr id="tr_dscmodalidade" style="display: <?if($tpcid != 13) echo 'none';?>;">
							<td class="subtitulodireita">Descrição da Modalidade:</td>
							<td><?=campo_texto('libdscoutras', 'N', 'S', 'Descrição da Modalidade', 60, 200, '', '','','','','id="libdscoutras"'); ?>
							</td>
						</tr>
						
						<tr bgcolor="#D0D0D0">
							<td></td>
							<td>
								<input type="button" value="Salvar" name="btnSalvar" <?=disabled($estadoAtual); ?> id="btnSalvar" onclick="gravarDadosGeraisCDO();" style="cursor: pointer;">
								<input type="button" value="Limpar Campos" name="btnLimpar" id="btnLimpar" onclick="window.location.href = 'elabrev.php?modulo=principal/dadosGeraisCDO&acao=A';" style="cursor: pointer;">
							</td>
						</tr>
					</table>
				<td valign="top"><?
					$docid = criarDocumento( $_SESSION['elabrev_var']['lbrid'] );
					wf_desenhaBarraNavegacao( $docid , array( 'lbrid' => $_SESSION['elabrev_var']['lbrid'], 'url' => $_SESSION['favurl'] ) );
				?></td>
			</tr>
			</table>
	</div>
</div>

<div id="div_aba_cdo" style="display: <?if($tcdid == 1 || !$tcdid) echo 'none';?>;">
<div id="abas" >
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Dados CDO</li>
			</ul>
		</div>
		<!-- Dados CDO -->
		<div id="aba_cdo" class="conteudoAba">
		
			<table width="15%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td align="left" width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<?php 
								$sql = "select coalesce(sum(lcdvalor),0) as total from elabrev.liberacaocdo where lcdstatus = 'A' and lbrid = ".$_SESSION['elabrev_var']['lbrid'];
								$totalCdo = $db->pegaUm($sql);
							?>
							<strong>Valor Total CDO R$:<div id="totalCdo" style="float:right"><?=number_format($totalCdo,2,",",".");?></div></strong>
						</td>
					</tr>
				</thead>
			</table>	
		
			<br>	
		
			<table width="80%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UG</strong></td>
						<td width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						<td width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PTRES</strong></td>
						<td width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Subação</strong></td>
						<td width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PI</strong></td>
						<td width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Natureza</strong></td>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$)</strong></td>
						<td width="81" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					</tr>
				</thead>
				<?// if ( $permissao_momento ) : ?>
					<tr bgcolor="#f7f7f7">
						<td width="90">
							<? 
							if($db->testa_superuser()) {
								$sql_financeiro_ug = "SELECT g.ungcod as codigo, g.ungdsc as descricao 
										FROM public.unidadegestora g WHERE g.ungstatus='A'";
								
							} else {
								$sql_financeiro_ug = "SELECT ung.ungcod as codigo, ung.ungdsc as descricao 
										FROM elabrev.usuarioresponsabilidade ur 
										INNER JOIN public.unidadegestora ung ON ung.ungcod=ur.ungcod 
										WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.rpustatus='A'";
							}
							$complemento = ' onkeypress="item_campo_focado( event, \'ug\' );" ';
							texto_popup( 'nova_ug', $sql_financeiro_ug, 'UG', 8, 10, '', '', $complemento );
							?>
						</td>
						<td width="90">
							<? 
							$sql_financeiro_acao = "SELECT
														ac.acacod as codigo,
														ac.prgcod || '.' || ac.acacod || '.' || ac.unicod || '.' || ac.loccod ||' - '|| ac.acadsc as descricao
													FROM monitora.pi_subacao sa
														inner join monitora.pi_subacaodotacao sad ON sad.sbaid = sa.sbaid
														inner join monitora.ptres p ON p.ptrid = sad.ptrid
														inner join monitora.acao ac on ac.acaid = p.acaid
														inner join monitora.pi_subacaounidade sau ON sau.sbaid = sa.sbaid
													WHERE ungcod is not null and ac.acastatus = 'A' and ac.unicod not in ('26101')
													GROUP BY ac.acadsc, ac.acacod, ac.prgcod, ac.unicod, ac.loccod
													ORDER BY ac.acadsc";
							$complemento = ' onkeypress="item_campo_focado( event, \'acao\' );" ';
							texto_popup( 'nova_acao', $sql_financeiro_acao, 'Ação', 8, 10, '', '', $complemento );
							?>
						</td>
						<td width="90">
							<?  
							$sql_financeiro_ptres = "select distinct ptres as codigo, '' as descricao from monitora.ptres";
							$complemento = ' onkeypress="item_campo_focado( event, \'ptres\' );" ';
							texto_popup( 'nova_ptres', $sql_financeiro_ptres, 'PTRES', 8, 10, '########', '', $complemento );
							?>
						</td>
						<td width="90">
							<?
							if($db->testa_superuser()) {
								$sql_financeiro_subacao = "select s.sbacod as codigo, s.sbatitulo as descricao 
									from monitora.pi_subacao s
									INNER JOIN monitora.pi_subacaounidade sau ON sau.sbaid = s.sbaid
									where sbastatus = 'A' and sau.ungcod is not null
									group by s.sbacod, s.sbatitulo, s.sbaid
									order by s.sbacod, s.sbatitulo";
								
							} else {
								$sql_financeiro_subacao = "select s.sbacod as codigo, s.sbatitulo as descricao 
									from monitora.pi_subacao s
									INNER JOIN monitora.pi_subacaounidade sau ON sau.sbaid = s.sbaid
									INNER JOIN elabrev.usuarioresponsabilidade ur ON ur.ungcod = sau.ungcod
									where sbastatus = 'A' 
									and sau.ungcod is not null
									and ur.usucpf = '".$_SESSION['usucpf']."' 
									and ur.rpustatus='A'
									group by s.sbacod, s.sbatitulo, s.sbaid
									order by s.sbacod, s.sbatitulo";
							}
							$complemento = ' onkeypress="item_campo_focado( event, \'subacao\' );" ';
							texto_popup( 'nova_subacao', $sql_financeiro_subacao, 'Subação', 8, 10, '', '', $complemento );
							?>
						</td>
						<td width="90">
							<?  
							if($db->testa_superuser()) {
								$sql_financeiro_pi = "select plicod as codigo, plititulo as descricao from monitora.pi_planointerno where pliano = '".$_SESSION['exercicio']."'";
								
							} else {
								$sql_financeiro_pi = "select pi.plicod as codigo, pi.plititulo as descricao 
													  from monitora.pi_planointerno pi 
													  INNER JOIN elabrev.usuarioresponsabilidade ur ON ur.ungcod = pi.ungcod
													  where pi.pliano = '".$_SESSION['exercicio']."'
													  and ur.usucpf = '".$_SESSION['usucpf']."' 
													  and ur.rpustatus='A'";
							}
							$complemento = ' onkeypress="item_campo_focado( event, \'pi\' );" ';
							texto_popup( 'nova_pi', $sql_financeiro_pi, 'PI', 8, 10, '', '', $complemento ); 
							?>
						</td>
						<td width="90">
							<?  
							$sql_financeiro_natureza = "select ndpcod as codigo, ndpdsc as descricao from naturezadespesa where edpcod != '00' and ndpstatus = 'A' order by ndpcod";
							$complemento = ' onkeypress="item_campo_focado( event, \'natureza\' );" ';
							texto_popup( 'nova_natureza', $sql_financeiro_natureza, 'Natureza da Despesa', 8, 10, '########', '', $complemento );
							?>
						</td>
						<td align="right">
							<? $complemento = ' onkeypress="item_campo_focado( event, \'valor\' );" '; ?>
							<?= campo_texto( 'nova_valor', 'N', 'S', '', 18, 14, '[###.]###,##', '', 'right', '', 0, ' id="nova_valor" onblur="MouseBlur(this);" ' . $complemento ); ?>
						</td>
						<td width="81" align="left">
							&nbsp;&nbsp;&nbsp;&nbsp;
							<a href="javascript:add_cdo();" title="Incluir" style="margin: 0; padding: 0 6px;">
								<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
							</a>
						</td>
					</tr>
				<? //else : ?>
					<!-- 
					<input type="hidden" value="" name="nova_uo" id="nova_uo"/>
					<input type="hidden" value="" name="nova_subacao" id="nova_subacao"/>
					<input type="hidden" value="" name="nova_natureza" id="nova_natureza"/>
					<input type="hidden" value="" name="novo_uso" id="novo_uso"/>
					<input type="hidden" value="" name="nova_fonte" id="nova_fonte"/>
					<input type="hidden" value="" name="novo_oc" id="novo_oc"/>
					<input type="hidden" value="" name="novo_valor" id="novo_valor"/>
					 -->
				<? //endif; ?>
				<tr>
					<td colspan="9">					
						<div class="div_rolagem" id="div_tabela_cdo" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
							<div id="listacdo">
								<?php 
									echo listaDadosCdo();								
								?>							
							</div>
						</div>
					</td>
				</tr>
			</table>
			<? //if ( $permissao_momento ) : ?>
			<!-- 	<br/><br/>
				<input type="button" name="salvar" value="Salvar" onclick="javascript:validar_cadastro();"/>
			 -->
			<? //endif; ?>
	</div>
</div>
</div>		

</form>


<script>
function gravarDadosGeraisCDO(){

	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "libnumprocesso";
	campos[1] 			= "libdescricao";
	campos[2] 			= "libvalortotalprevisto";
	campos[3] 			= "libvalorexercicioatual";
	campos[4] 			= "libvalorexercicioproximo";
	campos[5] 			= "libdataprevinicio";
	campos[6] 			= "tcdid";
	campos[7] 			= "tpcid";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "textarea";
	tiposDeCampos[2] 	= "texto";
	tiposDeCampos[3] 	= "texto";
	tiposDeCampos[4] 	= "texto";
	tiposDeCampos[5] 	= "texto";
	tiposDeCampos[6] 	= "select";
	tiposDeCampos[7] 	= "select";
	
	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
	
		if(!validaData($('libdataprevinicio') ) ) {
			alert('Data prevista de Início está no formato incorreto.');
			$('libdataprevinicio').focus();
			return false;
		}

		if($('tcdid').value != '1'){
			
			if(trim($('lbrnulibano').value) == ''){
				alert('Campo Nº Liberação CDO é Obrigatório.');
				$('lbrnulibano').focus();
				return false;
			}
			
		}
		else{
			$('lbrnulibano').value = '';
		}
		
		if($('tpcid').value == '13'){
			
			if(trim($('libdscoutras').value) == ''){
				alert('Campo Descrição da Modalidade é Obrigatório.');
				$('libdscoutras').focus();
				return false;
			}
		}
		else{
			$('libdscoutras').value = '';
		}

		var totalCdo = "<?=$totalCdo?>";
		if($('tcdid').value == '1' && parseFloat(totalCdo) > 0){
			
			if(!confirm('Ao alterar o Tipo do CDO para Novo, irá apagar todos os dados CDO. \nDeseja realmente fazer isto?')){
				return false;
			}
			
		}


		$('formulario').submit();
	}
}

function valorExercicioProximo(){
	var valorTotalPrevisto 		= $('libvalortotalprevisto').value;
	var valorExercicioAtual 	= $('libvalorexercicioatual').value;
	
	if(valorTotalPrevisto != '' && valorExercicioAtual != ''){
		valorTotalPrevisto = valorTotalPrevisto.replace(/\./gi,"");
		valorTotalPrevisto = valorTotalPrevisto.replace(/\,/gi,".");
		
		valorExercicioAtual = valorExercicioAtual.replace(/\./gi,"");
		valorExercicioAtual = valorExercicioAtual.replace(/\,/gi,".");
			
		var result = parseFloat(valorTotalPrevisto) - parseFloat(valorExercicioAtual);		
		$('libvalorexercicioproximo').value = mascaraglobal('###.###.###.###,##', result.toFixed(2));
	}
}


function mostraContratacao(v){

	var modalidade = document.getElementById("tr_dscmodalidade"); 
	
	if(v == '13'){
		modalidade.style.display = '';
	}
	else{
		modalidade.style.display = 'none';
	}
}

function mostraUgs(v){

	var modalidade = document.getElementById("div_aba_cdo"); 
	var nulib = document.getElementById("tr_nulib");
	
	if(v == '1'){
		modalidade.style.display = 'none';
		nulib.style.display = 'none';
	}
	else{
		modalidade.style.display = '';
		nulib.style.display = '';
	}
}

function add_cdo()
{
	
	// captura os campos do financeiro
	var ug = document.getElementById( 'nova_ug' );
	var acao = document.getElementById( 'nova_acao' );
	var ptres = document.getElementById( 'nova_ptres' );
	var subacao = document.getElementById( 'nova_subacao' );
	var pi = document.getElementById( 'nova_pi' );
	var natureza = document.getElementById( 'nova_natureza' );
	var valor = document.getElementById( 'nova_valor' );
	
	// realiza validação dos dados
	if ( !validaBranco( ug, 'Unidade Gestora' ) ) return;
	if ( !validaBranco( acao, 'Ação' ) ) return;
	if ( !validaBranco( ptres, 'PTRES' ) ) return;
	if ( !validaBranco( subacao, 'Subação' ) ) return;
	if ( !validaBranco( pi, 'PI' ) ) return;
	if ( !validaBranco( natureza, 'Natureza da Despesa' ) ) return;
	if ( !validaBranco( valor, 'Valor da Proposta' ) ) return;
	/*
	if ( !financeiro_ug_validas[ug.value] )
	{
		alert( 'Unidade Gestora inválida.' );
		ug.focus();
		ug.select();
		return;
	}
	if ( !financeiro_subacao_validas[subacao.value] )
	{
		alert( 'Subação inválida.' );
		subacao.focus();
		subacao.select();
		return;
	}
	if ( !financeiro_naturezas_validas[natureza.value] )
	{
		alert( 'Natureza de Despesa inválida.' );
		natureza.focus();
		natureza.select();
		return;
	}
	if ( !financeiro_usos_validos[uso.value] )
	{
		alert( 'Ident. de Uso inválido.' );
		uso.focus();
		uso.select();
		return;
	}
	if ( !financeiro_fontes_validas[fonte.value] )
	{
		alert( 'Fonte inválida.' );
		fonte.focus();
		fonte.select();
		return;
	}
	if ( !financeiro_ocs_validos[oc.value] )
	{
		alert( 'Ident. Op. de Crédito inválido.' );
		oc.focus();
		oc.select();
		return;
	}
	// insere os dados na tabela de financeiro
	if ( item_financeiro_adicionar( ug.value, subacao.value, natureza.value, uso.value, fonte.value, oc.value, valor.value ) == false )
	{
		return;
	}
	*/
	
	var total = "<?=$totalCdo?>";
	var valorprevisto = document.getElementById( 'libvalortotalprevisto' ).value;
	if(valorprevisto) {
		valorprevisto = valorprevisto.replace(/\./gi,"");
		valorprevisto = valorprevisto.replace(/\,/gi,".");
	}
	
	var valoradd = valor.value;
	if(valoradd) {
		valoradd = valoradd.replace(/\./gi,"");
		valoradd = valoradd.replace(/\,/gi,".");
		var totalgeraladd = parseFloat(total) + parseFloat(valoradd);
	}

	//alert(totalgeraladd +' - '+valorprevisto);
	if(totalgeraladd < valorprevisto) {

		var myajax = new Ajax.Request('elabrev.php?modulo=principal/dadosGeraisCDO&acao=A', {
	        method:     'post',
	        parameters: '&requisicao=cadDadosCdo&ug='+ug.value+'&acao='+acao.value+'&ptres='+ptres.value+'&subacao='+subacao.value+'&pi='+pi.value+'&natureza='+natureza.value+'&valor='+valor.value,
	        asynchronous: false,
	        onComplete: function (res){
				//$(listacdo).innerHTML = res.responseText;
				if(res.responseText == "OK"){
	        		alert("Dados gravados com sucesso.");
					location.href="elabrev.php?modulo=principal/dadosGeraisCDO&acao=A";
				}
				else{
					if(!res.responseText) res.responseText = "Erro ao gravar!";
					alert(res.responseText);
					//$(listacdo).innerHTML = res.responseText;
					return false;
				}
			
	        }
  		});
	}
	else{
		alert("Soma dos valores do CDO não pode ultrapassar o valor total previsto");
	}
}

function excluirDadosCdo(cod){

	if(confirm('Deseja realmente excluir este item?')){
		var myajax = new Ajax.Request('elabrev.php?modulo=principal/dadosGeraisCDO&acao=A', {
	        method:     'post',
	        parameters: '&requisicao=excluirDadosCdo&lcdid='+cod,
	        asynchronous: false,
	        onComplete: function (res){
				//$(listacdo).innerHTML = res.responseText;
				if(res.responseText == "OK"){
	        		alert("Dados excluidos com sucesso.");
					location.href="elabrev.php?modulo=principal/dadosGeraisCDO&acao=A";
				}
	        }
			});	
	}	
}


function item_campo_focado( event, campo )
{
	if ( event.keyCode != 13 )
	{
		return;
	}
	switch ( campo )
	{
		case 'ug':
			var campo = document.getElementById( 'nova_acao' );
			break;
		case 'acao':
			var campo = document.getElementById( 'nova_ptres' );
			break;
		case 'ptres':
			var campo = document.getElementById( 'nova_subacao' );
			break;
		case 'subacao':
			var campo = document.getElementById( 'nova_pi' );
			break;
		case 'pi':
			var campo = document.getElementById( 'nova_natureza' );
			break;
		case 'natureza':
			var campo = document.getElementById( 'nova_valor' );
			break;
		case 'valor':
			add_cdo();
			return;
		default:
			return;
	}
	campo.focus();
	campo.select();
}
</script>


