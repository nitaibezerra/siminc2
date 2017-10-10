<?
/*
if( !in_array($_SESSION['usucpf'], $cpf_liberados) ){
	echo '<script>
 			location.href = "fiesabatimento.php?modulo=inicio&acao=C";
 		  </script>';
	die;
}
*/  

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

 //$_SESSION['fiesabatimento_var']['cpfusuario'] = '';
 // $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
 //
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = ''; // CPF com dados em branco

//$_SESSION['fiesabatimento_var']['cpfusuario'] = "";
//$_SESSION['fiesabatimento_var']['cpfusuario'] = "";

 
//LISANE MARIA BOHN HENZ 
//$_SESSION['fiesabatimento_var']['cpfusuario'] = "";

//trata cpf
/*
if( strlen($_SESSION['fiesabatimento_var']['cpfusuario']) < 11){
	$_SESSION['fiesabatimento_var']['cpfusuario'] = str_pad($_SESSION['fiesabatimento_var']['cpfusuario'], 11, "0", STR_PAD_LEFT);
}
*/


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
		alert('Dados de identificação não preenchidos. Efetue a correção na Plataforma Freire.');
		window.location = 'fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A';
	</script>
<?php 
	die();
}
//}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$teste = testaProfessor();

$idoid = $db->pegaUm("SELECT idoid FROM fiesabatimento.identificacaodocente WHERE idostatus='A' and idocpf='".$_SESSION['fiesabatimento_var']['cpfusuario']."' order by 1");

if( $idoid ){
	//$docid = pegaDocidSolicitacao($idoid);
	//$sbaid = $db->pegaUm("SELECT sbaid FROM fiesabatimento.solicitacaoabatimento WHERE idoid = $idoid AND preid is null AND sbastatus = 'A'");

	$dadossbaid = $db->pegaLinha("SELECT sb.sbaid, sb.sbaanoinicio, sb.sbaanofim, sb.sbarenovacao FROM fiesabatimento.solicitacaoabatimento sb
								  WHERE sb.idoid = $idoid AND sb.sbarenovacao = false AND sb.sbastatus = 'A'");

	if($dadossbaid) extract($dadossbaid);

	if(!$sbaid){
		$sbaanoinicio = 2010;
		$sbaanofim = 2014;
	}else{
		$docid = pegaDocidSolicitacaoSbaid($sbaid);
	}

	$sbames = 12;
	$sbadescricao = "Solicitação - $sbaanoinicio à $sbaanofim";

	//ver($idoid);
	//ver($docid);
	//ver($sbaid,d);

	//verifica se foi rejeitada
	/*
	if(!$docid){
		$sql = "SELECT
					doc.docid
				FROM
					fiesabatimento.solicitacaoabatimento sab
				INNER JOIN workflow.documento doc ON doc.docid = sab.docid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				WHERE
					sbarenovacao = false
				AND sbastatus = 'A'
				AND esd.esdid = ".WF_FIES1_REJEITADO."
				AND idoid = $idoid
				";
		$docid = $db->pegaUm($sql);
	}
	*/

	if($docid){
		$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docid");
		$esdsc = $db->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = $esdid");
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

if($dados){
	$efetivoExercicio = "N";
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
   		
    	//if((int)$anoi > 2012){
		if((int)$anoi > $sbaanofim || ($anof < $sbaanoinicio && $anof)){
	    	unset($dados[$k]);
	    	$entrou = 1;
	    }
		if(!$anof) $efetivoExercicio = "S";
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


/*
 Possibilitar nova Solicitação de abatimento 1% ao professor que tenha efetuado Solicitação de abatimento com seleção NÃO para efetivo exercício
(na data da solicitação), esteja em situação Analisado / Enviado para processamento bancário e possua acréscimo de dados Atuação Profissional
(Rede pública de educação básica) na Plataforma Freire;
 */
if($efetivoExercicio == "N" && $esdid == WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO){

	unset($teste);
	unset($teste2);

	unset($sbaid);
	unset($docid);
	unset($esdid);
	unset($esdsc);
	unset($dados);
	$disable = false;

	$sbaanoinicio = 2013;
	$sbaanofim = 2014;
	$sbames = 12;
	$sbadescricao = "Solicitação - $sbaanoinicio à $sbaanofim";

	$dados = pegarAtuacaoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);

	if($dados){
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

			//if((int)$anoi > 2012){
			if((int)$anoi > $sbaanofim || ($anof < $sbaanoinicio && $anof)){
				unset($dados[$k]);
				$entrou = 1;
			}
		}

		if($entrou==1){
			$dados  = array_values($dados);
		}
	}

	$teste = calculaMeses( $dados, $sbaanoinicio, $sbaanofim, $sbames );
	$dadosAtu = pegarAtuacaoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);

}
/*
 FIM nova Solicitação
 */

?>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>

<?php if( $idoid == '' ){?>
$('[title="Cancelar Abatimento"]').attr('href',' ');
$('[title="Cancelar Abatimento"]').click(function(){<?=$onclick ?>});
<?php }?>
function confirmarSolicitacao() {

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
	/*
	if(document.getElementById('clausula_e').checked==false) {
		alert('Você deve selecionar as declarações da tela para, em seguida, ser enviada a solicitação.');
		return false;
	}*/

	<? if(!$dadosAtu[0]) : ?>
	alert('Dados de atuação profissional não preenchidos. Efetue a correção na Plataforma Freire.');
	return false;
	<? endif; ?>

	<? if($teste['meses'] < 12) : ?>
	alert('Para solicitar o abatimento é necessário ter jornada de, no mínimo, 12 (doze) meses de trabalho \n'+ 
		  'ininterrupto com no mínimo 20 (vinte) horas semanais como docente na rede pública de educação básica.');
	return false;
	<? endif; ?>
	
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
	<input type="hidden" name="sbarenovacao" value="f" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Orientações :</b></td>
		<td>
		<!-- 
		<p>Confira as informações do quadro "Atuação Profissional" e, se necessário, faça a(s) correção(ões) na Plataforma Freire, <a href="http://freire.homolog.capes.gov.br/ssd/servlet/" target="_blank">clicando aqui</a>.
		<br>Em seguida, para confirmar a solicitação do abatimento,  selecione as afirmativas do item “Declaração”, ratificando conhecimento e concordância com o seu teor, para então acionar a opção “Confirmar solicitação”.</p>
		-->
		<p>Confira as informações do quadro "Atuação Profissional" e, se necessário, faça a(s) correção(ões) na Plataforma Freire, <a href="<?=LINK_FREIRE ?>" target="_blank">clicando aqui</a>.
		<br>Em seguida, para confirmar a solicitação do abatimento,  selecione as afirmativas do item “Declaração”, ratificando conhecimento e concordância com o seu teor, para então acionar a opção “Confirmar solicitação”.</p>
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
			&nbsp;Quantidade mínima necessária: <? echo campo_texto("qtdmesestrabalhadosiniterruptamente", "N", "N", "Quantidade mínima de meses trabalhados ininterruptamente", 4, 3, "###", "", "", "", 0, "", "", "12"); ?> 
			&nbsp;Quantidade de meses apurados: <? echo campo_texto("sbaqmtsolicitado", "N", "N", "Quantidade de meses trabalhados (QMT)", 4, 3, "###", "", "", "", 0, "", "", $teste['meses']); ?>
			<?php if( $esdid == WF_FIES1_APROVADA || $esdid == WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO ){?>
			&nbsp;Quantidade de meses aprovados: <? echo campo_texto("sbaqmtsolicitado", "N", "N", "Quantidade de meses trabalhados (QMT)", 4, 3, "###", "", "", "", 0, "", "", $teste2['meses']); ?><br>
			<?php }?>
			&nbsp;Quantidade de meses apurados atende a quantidade mínima necessária? <b><? if($teste['meses']>=12) : ?>Sim<input type="hidden" name="sbasolicitacaoatendida" value="TRUE"><? else : ?>Não<input type="hidden" name="sbasolicitacaoatendida" value="FALSE"><? endif; ?></b></td>
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
			a)	li, compreendi e concordo com as condições e os normativos que regulam a concessão do abatimento e que as informações prestadas são verdadeiras, assim como atendo as condições exigidas para solicitar o abatimento.
			<p><input type="checkbox" name="clausula_b" id="clausula_b" value="b" <? if($disable) echo "checked disabled"; ?> > 
			b)	a concessão do abatimento depende de aprovação do(s) Secretário(s) de Educação para a(s) escola(s) a(s) qual(is) presto o efetivo exercício da docência.
			<p><input type="checkbox" name="clausula_c" id="clausula_c" value="c" <? if($disable) echo "checked disabled"; ?> > 
			c)	a não aprovação da solicitação de abatimento pelo(s) respectivo(s) Secretário(s) de Educação da escola vinculada em que presto o exercício da docência implicará na não concessão do abatimento, seja por decurso do prazo de validação, período do exercício da docência menor do que 12 meses ininterruptos, carga horária semanal inferior a 20 horas, não pertencer ao quadro permanente ou temporário da escola, não exercer a docência ou não ter vínculo empregatício com a escola vinculada à Secretaria.
			<p><input type="checkbox" name="clausula_d" id="clausula_d" value="d" <? if($disable) echo "checked disabled"; ?> > 
			d)	imediatamente após a perda da condição de docente, motivada por aposentadoria ou qualquer outro fato que impeça a condição do abatimento ou da carga horária semanal de, no mínimo, 20 horas na rede pública da educação básica, registrarei a ocorrência e solicitarei o cancelamento do abatimento, de acordo com as normas que regulam o abatimento.
<!--			<p><input type="checkbox" name="clausula_e" id="clausula_e" value="e" <? if($disable) echo "checked disabled"; ?> > 
 				e) imediatamente após a perda da condição de docente, motivada por aposentadoria ou qualquer outro fato que impeça a condição do abatimento ou da carga horária semanal de, no mínimo, 20 horas na rede pública da educação básica, registrarei a ocorrência e solicitarei o cancelamento do abatimento, de acordo com as normas que regulam o abatimento.</p> -->
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
			<?
			if(!$docid || $esdid == WF_FIES1_EM_SOLICITACAO_PELO_PROFESSOR || $esdid == WF_FIES1_REENVIO ){ ?>
				<input type="button" name="confirmar" value="Confirmar Solicitação" onclick="confirmarSolicitacao();" >
			<? }elseif( $esdid == WF_FIES1_APROVADA ){ ?>
			<?php if( date('Ymd') < 20130503 ){?>
				<label style="color:red">O envio da solicitação ao Agente Financeiro estará disponível em 03/05/2013.</label>
			<?php }elseif( !testaHorario() ){?>
				<label style="color:red">Este serviço funciona apenas em horário comercial.</label>
			<?php }?>
				<input type="button" name="confirmar" value="Enviar Requisição" class="enviarReq" <?=(testaHorario() ) ? '' : 'disabled="disabled"' ?> >
			<?php } ?> 
			<input type="button" name="voltar" value="Voltar" onclick="window.location='fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A';">
			<? if( $idoid ) : ?>
				<!-- <a class="historico" style="cursor:pointer">Histórico</a>  -->
			<? endif; ?> 
		</td>
	</tr>
</table>
</form>