<?
function testaHorario(){
	
	if(
		date('His') > '80000' && date('His') < '120000' ||
		date('His') > '140000' && date('His') < '180000' ){
		return true;
	}
	return false;
}

if($_REQUEST['req2']) {
	$_REQUEST['req2']($_REQUEST);
	exit;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//testa_prazo_solicitacao();

// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = ''; // CPF com dados em branco

$dadosUsu = pegarInfoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);
$dadosUsu = $dadosUsu ? $dadosUsu : Array();
//foreach($dadosUsu as $dado){
//if( $dado == '' ){
if ((trim($dadosUsu[nu_cpf])== false) || (trim($dadosUsu[nu_rg])== false)  || 
	 (trim($dadosUsu[nu_cep])== false) || (trim($dadosUsu[ds_numero])== false) ||
 	 (trim($dadosUsu[ds_bairro])== false) || (trim($dadosUsu[nu_ddd])== false)    ||	    
 	 (trim($dadosUsu[no_pessoa])== false) || (trim($dadosUsu[ds_logradouro])== false) || 
 	 (trim($dadosUsu[nu_telefone])== false) || (trim($dadosUsu[dt_nascimento])== false) ||
 	 (trim($dadosUsu[no_municipio_acento])== false)   || (trim($dadosUsu[ds_contato_eletronico])== false)
 	 ){
?>
	<script>
		//alert('Dados de identificação não preenchidos. Efetue a correção na Plataforma Freire.');
		//window.location = 'fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A';
	</script>
<?php 
	//die();
}
//}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$teste = testaProfessor();

$dadosRenovacao = $db->pegaLinha("SELECT pdadatainiciorenovacao, pdadatafimrenovacao, pdaano FROM fiesabatimento.parametrosabatimento WHERE pdastatus='A'");

if(!$dadosRenovacao['pdaano'] || !$dadosRenovacao['pdadatainiciorenovacao'] || !$dadosRenovacao['pdadatafimrenovacao']){
	echo '<script>
				alert("Período de Renovação do Abatimento, ainda não esta aberto.");
				window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
			  </script>';
	die();
}

if($dadosRenovacao['pdadatainiciorenovacao'] && $dadosRenovacao['pdadatafimrenovacao']){
	$dtAtual = (int) date("Ymd");
	$dtinirenovacao = (int) str_replace("-","",$dadosRenovacao['pdadatainiciorenovacao']);
	$dtfimrenovacao = (int) str_replace("-","",$dadosRenovacao['pdadatafimrenovacao']);
	$anoFimRenovacao = $dadosRenovacao['pdaano'];

	if($dtAtual < $dtinirenovacao){
		echo '<script>
				alert("Período de Renovação do Abatimento, ainda não esta aberto.");
				window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
			  </script>';
		die();
	}

	if($dtAtual > $dtfimrenovacao){
		echo '<script>
				alert("Período expirado para Renovação do Abatimento.");
				window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
			  </script>';
		die();
	}
}

$idoid = $db->pegaUm("SELECT idoid FROM fiesabatimento.identificacaodocente WHERE idostatus='A' and idocpf='".$_SESSION['fiesabatimento_var']['cpfusuario']."' order by 1");

if( $idoid ){
	
	
	//verifica se ja fez alguma solicitação
	$dadoSbaidSolicitacao = $db->pegaLinha("SELECT s.sbaid, d.esdid, s.sbaanoinicio, s.sbaanofim FROM fiesabatimento.solicitacaoabatimento s
								  inner join workflow.documento d on d.docid = s.docid and d.esdid = ".WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO."
								  WHERE s.idoid = $idoid AND s.sbarenovacao = false AND s.sbastatus = 'A'");

	if(!$dadoSbaidSolicitacao['sbaid']){
		echo '<script>
				alert("Para fazer a Solicitação de Renovação do Abatimento, é necessário ter pelo menos uma Solicitação de Abatimento enviada para Processamento Bancário em que tenha selecionado estar em efetivo exercício.");
				window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
			  </script>';
		die();
	}else {

		//$docid = pegaDocidSolicitacao($idoid,$preid);
		//$sbaid = $db->pegaUm("SELECT sbaid FROM fiesabatimento.solicitacaoabatimento WHERE idoid = $idoid AND preid=$preid and sbastatus = 'A'");

		//verifica efetivo exercicio
		$efetivoExercicio = $db->pegaUm("SELECT count(atpid) as total FROM fiesabatimento.atuacaoprofissional
				                         WHERE atpstatus='A'
				                         and atpprofefetivoexercicio = true
				                         and ( (atpdatainicio <= '{$dadoSbaidSolicitacao['sbaanofim']}-12-31' and atpdatafim >= '{$dadoSbaidSolicitacao['sbaanoinicio']}-01-01' ) OR (atpdatainicio <= '{$dadoSbaidSolicitacao['sbaanofim']}-12-31' and atpdatafim is null) )
				                         and sbaid = ".$dadoSbaidSolicitacao['sbaid']);

		if($efetivoExercicio == 0){
			echo '<script>
					alert("Você não está em Efetivo Exercicio na(s) Escola(s). Por favor, verifique com o Gestor do sistema!");
					window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
				  </script>';
			die();
		}

		//verifica se ja fez alguma renovação
		$dadoSbaidRenovacao = $db->pegaLinha("SELECT s.sbaid, d.esdid, s.sbaanofim FROM fiesabatimento.solicitacaoabatimento s
								  inner join workflow.documento d on d.docid = s.docid and d.esdid = ".WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO."
								  WHERE s.idoid = $idoid AND s.sbarenovacao = true AND s.sbastatus = 'A' order by s.sbaid DESC");

		if($dadoSbaidRenovacao['sbaid']){

			/*
			if($dadoSbaidRenovacao['sbaanofim'] > $anoFimRenovacao){
				echo '<script>
						alert("Você já fez a renovação do ano exercício: "'.$anoFimRenovacao.'". Por favor, consulte o histórico de solicitações!");
						window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
					  </script>';
				die();
			}
			*/

			//atribui ano fim
			$dadoSbaidSolicitacao['sbaanofim'] = $dadoSbaidRenovacao['sbaanofim'];

		}else{

			//verifica se existe alguma renovação em andamento
			$existeRenovacao = $db->pegaLinha("SELECT s.sbaanofim FROM fiesabatimento.solicitacaoabatimento s
								  inner join workflow.documento d on d.docid = s.docid and d.esdid not in(".WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO.")
								  WHERE s.idoid = $idoid AND s.sbarenovacao = true AND s.sbastatus = 'A' order by s.sbaid DESC");

			if( $existeRenovacao['sbaanofim'] && ((int)$existeRenovacao['sbaanofim'] < (int)$anoFimRenovacao) ){
				$existeAno = $existeRenovacao['sbaanofim'];
				echo '<script>
						alert("Você possui uma renovação pendente do ano exercício: '.$existeAno.'. Por favor, aguarde ser aprovada, para liberar o próximo ano de renovação!");
						window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
					  </script>';
				die();
			}

		}


		$SbaidMax = $db->pegaUm("SELECT max(sb.sbaid) as sbaid FROM fiesabatimento.solicitacaoabatimento sb
									  WHERE sb.idoid = $idoid
									  AND sb.sbaanofim = '$anoFimRenovacao'
									  AND sb.sbarenovacao = true
									  AND sb.sbastatus = 'A'");

		if($SbaidMax) {
			$dadoSbaidRenovacao = $db->pegaLinha("SELECT sbaid, sbaanoinicio, sbaanofim FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = " . $SbaidMax);
			if ($dadoSbaidRenovacao) extract($dadoSbaidRenovacao);
		}

		if (!$sbaid) {
			$sbaanoinicio = $dadoSbaidSolicitacao['sbaanofim']+1;
			$sbaanofim = $anoFimRenovacao;

			if($sbaanoinicio > $sbaanofim){
				echo '<script>
						alert("Você não possui Renovação de Abatimento, pois o Ano fim da Solicitação é igual ao ano fim da Renovação.");
						window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
					  </script>';
				die();
			}

		} else {

			if((int)$sbaanoinicio > (int)$anoFimRenovacao){
				echo '<script>
						alert("Você já fez a renovação do ano exercício: '.$anoFimRenovacao.'. Por favor, consulte o histórico de solicitações!");
						window.location = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
					  </script>';
				die();
			}

			$docid = pegaDocidSolicitacaoSbaid($sbaid);
		}

		$sbames = 12;
		$sbadescricao = "Renovação - $sbaanoinicio à $sbaanofim";

		if ($docid) {
			$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docid");
			$esdsc = $db->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = $esdid");
		}
	}
}else{
	$onclick = "alert('Você não possui Solicitação de Abatimento.');";
}

$arrEstados = Array(WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL,WF_FIES1_APROVADA);

if( !in_array($esdid,$arrEstados) ){

	if($dadosUsu){
		$dadosUsu['dt_nascimento'] = str_replace("/","-",$dadosUsu['dt_nascimento']);
		$nasc = explode('-',$dadosUsu['dt_nascimento']);
		if(strlen($nasc[2]) == 4) $dt_nasc=$nasc[2].'-'.$nasc[1].'-'.$nasc[0];
		if(strlen($nasc[0]) == 4) $dt_nasc=$nasc[0].'-'.$nasc[1].'-'.$nasc[2];
		$dadosUsu['dt_nascimento'] = $dt_nasc;
	}
	//dbg($dadosUsu,1);

	//$erro .= testeWS( $dadosUsu );
	if($erro!=''){
		echo "<script>
		alert('".$erro."');
		window.location = 'fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A';
		</script>";
		die();
	}
}


if( $sbaid && ( $esdid != WF_FIES1_EM_SOLICITACAO_PELO_PROFESSOR && $esdid != WF_FIES1_REENVIO ) ){
	$dados = pegaAtuacaoSolicitacao($sbaid, $sbaanoinicio, $sbaanofim, $sbarenovacao, $sbames );

	if( $esdid == WF_FIES1_APROVADA || $esdid == WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO ){
		$periodos = pegaPeriodosAtuacao( $sbaid );
		$teste2 = calculaMeses( $periodos, $sbaanoinicio, $sbaanofim, $sbames );
	}
}else{

	$arrDecurso = Array(WF_FIES1_REENVIO,WF_FIES1_REENVIO_PRAZO);
	if( in_array($esdid,$arrDecurso) ){
		testa_prazo_reenvio_solicitacao( $sbaid );
	}

	$dados = pegarAtuacaoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);

}


//faz filtro no array
if($dados){
	//$efetivoExercicio = "N";
	foreach ($dados as $k => $v) {
		//dbg($v['dt_inicio']);
		unset($anoi);
		unset($anof);
		$v['dt_inicio'] = str_replace("/","-",$v['dt_inicio']);
		$tempDatai = explode('-', $v['dt_inicio']);
		$v['dt_fim'] = str_replace("/","-",$v['dt_fim']);
		$tempDataf = explode('-', $v['dt_fim']);
		//dbg($tempData[2]);
		if(strlen($tempDatai[2]) == 4) $anoi=$tempDatai[2];
		if(strlen($tempDatai[0]) == 4) $anoi=$tempDatai[0];
		if(strlen($tempDataf[2]) == 4) $anof=$tempDataf[2];
		if(strlen($tempDataf[0]) == 4) $anof=$tempDataf[0];

		if((int)$anoi > $sbaanofim || ($anof < $sbaanoinicio && $anof)){
			unset($dados[$k]);
			$entrou = 1;
		}
		//if(!$anof) $efetivoExercicio = "S";
	}

	if($entrou==1){
		$dados  = array_values($dados);
	}
}

$teste = calculaMeses( $dados, $sbaanoinicio, $sbaanofim, $sbames );

$dadosAtu = pegarAtuacaoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);

if( !$teste['meses'] ) $teste['meses'] = '0';

$disable = true;
$cancelaAtuacao = false;

if(!$docid || $esdid == WF_FIES1_EM_SOLICITACAO_PELO_PROFESSOR || $esdid == WF_FIES1_REENVIO ) {
	$disable = false;
}
if( $esdid == WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL ){
	$cancelaAtuacao = true;
}

//FIM VERIFICA
?>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>

<?php if( $idoid == '' ){?>
//$('[title="Cancelar Renovação"]').attr('href',' ');
//$('[title="Cancelar Renovação"]').click(function(){<?//=$onclick ?>});
<?php }?>
function confirmarRenovacao() {

	if(document.getElementById('clausula_a').checked==false) {
		alert('Você deve selecionar as declarações da tela para, em seguida, ser enviada a solicitação.');
		return false;
	}
	
	if(document.getElementById('clausula_b').checked==false) {
		alert('Você deve selecionar as declarações da tela para, em seguida, ser enviada a solicitação.');
		return false;
	}
	
	if(document.getElementById('clausula_c').checked==false) {
		alert('Você deve selecionar as declarações da tela para, em seguida, ser enviada a solicitação.');
		return false;
	}
	
	if(document.getElementById('clausula_d').checked==false) {
		alert('Você deve selecionar as declarações da tela para, em seguida, ser enviada a solicitação.');
		return false;
	}
	
	if(document.getElementById('clausula_e').checked==false) {
		alert('Você deve selecionar as declarações da tela para, em seguida, ser enviada a solicitação.');
		return false;
	}

	<? if(!$dadosAtu[0]) : ?>
	alert('Dados de atuação profissional não preenchidos. Efetue a correção na Plataforma Freire.');
	return false;
	<? endif; ?>

	<? if($teste['meses'] < 12) : ?>
	//alert('Para solicitar o abatimento é necessário ter jornada de, no mínimo, 12 (doze) meses de trabalho \n'+'ininterrupto com no mínimo 20 (vinte) horas semanais como docente na rede pública de educação básica.');
	//return false;
	<? endif; ?>

	var existe = 0;
	$('[name^=efetivo_exercicio]').each(function() {
		if(this.checked && this.value == 'true'){
			existe = 1;
		}
	});

	if(existe==0) {
		alert('Para fazer a renovação é preciso estar em efetivo exercício em alguma escola.');
		return false;
	}


	document.getElementById('formulario').submit();
	
}

jQuery(document).ready(function(){
	
	jQuery('.historico').click(function(){
		var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=<?=$docid?>';
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	});

	<?php if( $sbaid && ( $esdid == WF_FIES1_APROVADA ) ){ ?>
	jQuery('.enviarReq').click(function(){
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "req2=testeWSAjax",
			async: false,
			success: function(msg){
				if( msg != '' ){
					alert(msg);
					return false;
				}else{
					if( confirm('Deseja confirmar o envio?') ){
						jQuery('#requisicao').val('enviarRequisicao');
						jQuery('#formulario').submit();
					}
				}
			}
		});
	});
	<?php } ?>
	
});

</script>

<form name="formulario" method="post" id="formulario">

	<input type="hidden" name="requisicao" id="requisicao" value="confirmarSolicitacao">
	<input type="hidden" name="idoid" value="<?=$idoid ?>" />
	<input type="hidden" name="atpid" id="atpid" value="" />
	<input type="hidden" name="sbaid" value="<?=$sbaid ?>" />
	<input type="hidden" name="sbaanoinicio" value="<?=$sbaanoinicio?>" />
	<input type="hidden" name="sbaanofim" value="<?=$sbaanofim?>" />
	<input type="hidden" name="sbarenovacao" value="t" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Orientações :</b></td>
		<td>
		<!-- 
		<p>Confira as informações do quadro "Atuação Profissional" e, se necessário, faça a(s) correção(ões) na Plataforma Freire, <a href="http://freire.homolog.capes.gov.br/ssd/servlet/" target="_blank">clicando aqui</a>.
		<br>Em seguida, para confirmar a solicitação do abatimento,  selecione as afirmativas do item “Declaração”, ratificando conhecimento e concordância com o seu teor, para então acionar a opção “Confirmar solicitação”.</p>
		-->
		<br>
		Confira as informações do quadro "Atuação Profissional" e, se necessário, faça a(s) correção(ões) na Plataforma Freire, <a href="<?=LINK_FREIRE ?>" target="_blank">clicando aqui</a>.
		<br>Em seguida, para confirmar a solicitação de renovação do abatimento, selecione as afirmativas do item “Declaração”, ratificando conhecimento e concordância com o seu teor, para então acionar a opção “Confirmar solicitação”.</p>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Referência :</b></td>
		<td>
			<?=$sbadescricao?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Atuação Profissional (Rede pública de educação básica):</td>
		<td>
			<?php
			if( $sbaid && ( $esdid != WF_FIES1_EM_SOLICITACAO_PELO_PROFESSOR && $esdid != WF_FIES1_REENVIO ) ){
				htmlAtuacaoSolicitacao($sbaid, true, $cancelaAtuacao, $sbaanoinicio, $sbaanofim, $sbarenovacao, $sbames );
			}else{
				htmlAtuacaoUsuario($_SESSION['fiesabatimento_var']['cpfusuario'], $sbaanoinicio, $sbaanofim, $sbarenovacao, $sbames);
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Total de meses trabalhados(ininterruptos) :</td>
		<td>
			&nbsp;Quantidade mínima necessária: <? echo campo_texto("qtdmesestrabalhadosiniterruptamente", "N", "N", "Quantidade mínima de meses trabalhados ininterruptamente", 4, 3, "###", "", "", "", 0, "", "", "1"); ?>
			&nbsp;Quantidade de meses apurados: <? echo campo_texto("sbaqmtsolicitado", "N", "N", "Quantidade de meses trabalhados (QMT)", 4, 3, "###", "", "", "", 0, "", "", $teste['meses']); ?>
			<?php if( $esdid == WF_FIES1_APROVADA || $esdid == WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO ){?>
				&nbsp;Quantidade de meses aprovados: <? echo campo_texto("sbaqmtsolicitado", "N", "N", "Quantidade de meses trabalhados (QMT)", 4, 3, "###", "", "", "", 0, "", "", $teste2['meses']); ?><br>
			<?php }?>
			&nbsp;Quantidade de meses apurados atende a quantidade mínima necessária? <b><? if($teste['meses']>=1) : ?>Sim<input type="hidden" name="sbasolicitacaoatendida" value="TRUE"><? else : ?>Não<input type="hidden" name="sbasolicitacaoatendida" value="FALSE"><? endif; ?></b></td>
	</tr>
	<?php
		if( $sbaid && ( $esdid == WF_FIES1_APROVADA ) ){
	?>
	<tr>
		<td class="SubTituloDireita">Declaração :</td>
		<td>
			<p>Você está no ultimo passo para garantir o seu beneficio. </p>
			<p>Clicando no botão abaixo o <?php echo SIGLA_SISTEMA; ?> enviará uma requisição para o sistema bancário solicitando o abatimento devido 
				em seu contrato e, se for o caso, a suspenção da cobrança das parcelas mensais.</p>
		</td>
	</tr>
	<?php 
		}else{
	?>
	<tr>
		<td class="SubTituloDireita">Declaração :</td>
		<td>
			<p>Declaro, sob as penas da lei que:</p>
			<p><input type="checkbox" name="clausula_a" id="clausula_a" value="a" <? if($disable) echo "checked disabled"; ?> > 
			a)	li, compreendi e concordo com as condições e os motivos que regulam a renovação da concessão do abatimento e que as informações prestadas são verdadeiras, assim como atendo as condiuções exigidas para a renovação da concessão do abatimento.
			<p><input type="checkbox" name="clausula_b" id="clausula_b" value="b" <? if($disable) echo "checked disabled"; ?> > 
			b)	a renovação da concessão do abatimento depende de aprovação do(s) Secretário(s) de Educação para a(s) escola(s) a(s) qual(is) preste o efetivo exercício a docência.
			<p><input type="checkbox" name="clausula_c" id="clausula_c" value="c" <? if($disable) echo "checked disabled"; ?> > 
			c)	a não aprovação da solicitação de renovação da concessão do abatimento pelo(s) respectivo(s) Secretário(s) de Educação da escola vinculada e que presto o exercício da docência implicará na não concessão da renovação do abatimento, seja por decurso do prazo de validação, período do ecercício da docência menor do que 12 meses initerruptos, carga horária semanal inferior a 20 horas, não pertencer ao quadro permanente ou temporário da escola, não exercer a docência ou não ter vínculo empregatício com a escola vinculada a Secretaria.
			<p><input type="checkbox" name="clausula_d" id="clausula_d" value="d" <? if($disable) echo "checked disabled"; ?> > 
			d)	estou ciente de que periodicamente deverei solicitar a renovação do abatimento e de que a sua não execução poderá implicar na suspensão da concessão do abatimento. 
			<p><input type="checkbox" name="clausula_e" id="clausula_e" value="e" <? if($disable) echo "checked disabled"; ?> > 
			e)	iminentemente após a perda da condição de docente, motivada por aposentadoria ou qualquer outro fato que impeça a condição da concessão do abatimento ou da carga horária semanal de, no minimo, 20 horas na rede pública de educação básica, registrarei a ocorrência e solicitarei o cancelamento do abatimento, de acordo com as normas que regulam o abatimento. 
			<!--
				<p><input type="checkbox" name="clausula_e" id="clausula_e" value="e" <? if($disable) echo "checked disabled"; ?> > 
 				e) imediatamente após a perda da condição de docente, motivada por aposentadoria ou qualquer outro fato que impeça a condição do abatimento ou da carga horária semanal de, no mínimo, 20 horas na rede pública da educação básica, registrarei a ocorrência e solicitarei o cancelamento do abatimento, de acordo com as normas que regulam o abatimento.</p> 
 			-->
		</td>
	</tr>
	<?php 
		}
	?>
	<?php if( $esdsc != '' ){?>
	<tr>
		<td class="SubTituloDireita">Situação da Solicitação :</td>
		<td><?=$esdsc  ?></td>
	</tr>
	<?php }?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<? if(!$docid || $esdid == WF_FIES1_EM_SOLICITACAO_PELO_PROFESSOR || $esdid == WF_FIES1_REENVIO ){ ?>
				<?if($teste['meses']>0){ ?>
					<input type="button" name="confirmar" value="Solicitar Renovação" onclick="confirmarRenovacao();" >
				<?}else{?>
					<input type="button" name="confirmar" value="Solicitar Renovação" onclick="alert('Quantidade Atual deve ser maior que zero!');" >
				<?}?>
			<? }elseif( $esdid == WF_FIES1_APROVADA ){ ?>
				<?php if( date('Ymd') < 20130503 ){?>
					<label style="color:red">O envio da solicitação ao Agente Financeiro estará disponível em 03/05/2013.</label>
				<?php }elseif( !testaHorario() ){?>
					<label style="color:red">Este serviço funciona apenas em horário comercial.</label>
				<?php }else{?>
					
					<input type="button" name="confirmar" value="Enviar Requisição" class="enviarReq" <?=(testaHorario() ) ? '' : 'disabled="disabled"' ?> >
					
				<?php } ?> 
			<?php } ?> 
			<input type="button" name="voltar" value="Voltar" onclick="window.location='fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A';">
			<? if( $idoid ) : ?>
				<!-- <a class="historico" style="cursor:pointer">Histórico</a>  -->
			<? endif; ?> 
		</td>
	</tr>
</table>
</form>
