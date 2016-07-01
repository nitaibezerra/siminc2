<?php

if( $_REQUEST['mdsiddelete'] ){
	$sql = "update proinfantil.mdssuplementacao set mdsstatus = 'I' where mdsid = {$_REQUEST['mdsiddelete']}";
	$db->executar($sql);
	$db->commit();
	$db->sucesso('suplementacaomds/listaCreche');
	exit();
}

if($_REQUEST['muncod']){
	$_SESSION['proinfantil']['mds']['muncod'] = $_REQUEST['muncod']; 
}

if($_REQUEST['requisicao'] == 'salvarQuantidades'){
	
	if( is_array($_POST['entcodent']) ) {
		foreach ($_POST['entcodent'] as $key => $entcodent) {
			$mdsquantidadepbfparcial = $_POST['mdsquantidadepbfparcial'][$entcodent];
			$mdsquantidadepbfintegral = $_POST['mdsquantidadepbfintegral'][$entcodent];
			
			$prcqtdalunoinfantilintegral = $_POST['prcqtdalunoinfantilintegral'][$entcodent];
			$prcqtdalunoinfantilparcial = $_POST['prcqtdalunoinfantilparcial'][$entcodent];
			
			$mdsquantidadepbfparcial = ( $mdsquantidadepbfparcial ? $mdsquantidadepbfparcial : '0' );
			$mdsquantidadepbfintegral = ( $mdsquantidadepbfintegral ? $mdsquantidadepbfintegral : '0' );
			
			$prcid = $_POST['prcid'][$entcodent];
			$mdsid = $_POST['mdsid'][$entcodent];
	
			if( $mdsquantidadepbfintegral > $prcqtdalunoinfantilintegral ){
				echo "<script>
					alert('A Qtd. de crianças na Creche Integral não pode ser menor que a Qtd. Crianças de 0 a 48 meses - Bolsa Família Integral');
					window.history.back(-1);
				</script>";
				exit();
			} 
			
			if( $mdsquantidadepbfparcial > $prcqtdalunoinfantilparcial ){
				echo "<script>
					alert('A Qtd. de crianças na Creche Parcial não pode ser menor que a Qtd. Crianças de 0 a 48 meses - Bolsa Família Parcial');
					window.history.back(-1);
				</script>";
				exit();
			}
			
			//$mdsid = $db->pegaUm("SELECT mdsid FROM proinfantil.mdssuplementacao WHERE prcid = '{$prcid}' ORDER BY mdsid DESC"); 
			
			if($mdsid == '0'){
				$sql = "insert into proinfantil.mdssuplementacao
								(mdsdtinclusao, usucpfinclusao, mdsstatus, entcodent, mdsquantidadepbfparcial, mdsquantidadepbfintegral, prcid)
							values
								(now(), '{$_SESSION['usucpf']}', 'A', '{$entcodent}', {$mdsquantidadepbfparcial}, {$mdsquantidadepbfintegral}, {$prcid}); ";
				$db->executar($sql);
			}else{
				$sql = "update proinfantil.mdssuplementacao set 
							mdsquantidadepbfparcial = {$mdsquantidadepbfparcial},
							mdsquantidadepbfintegral = {$mdsquantidadepbfintegral}
						where
							mdsid = {$mdsid}; ";
				$db->executar($sql);
			}
		}
		if($db->commit()){
			$db->sucesso('suplementacaomds/listaCreche', '');
		} else {
			$db->insucesso('Não foi possível completar a operação!', '', 'suplementacaomds/listaCreche&acao=A');
		}
	} else {
		$db->insucesso('Não foi possível completar a operação!', '', 'suplementacaomds/listaCreche&acao=A');
	}
	exit();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
include  "_funcoes_mds.php";
echo "<br/>";
?>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" id="btnVoltar" />
		</td>
		<td class="TituloTela">Suplementação MDS</td>
	</tr>
</table>

<br/>
<?php 
$perfis = pegaPerfil($_SESSION['usucpf']);

$sql = "SELECT 	cpmid 
		FROM 	proinfantil.mdsdadoscriancapormunicipio cpm		 
		WHERE	muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' and cpm.cpmano = ({$_SESSION['exercicio']} - 1)";

$cpmid = $db->pegaUm($sql);

$docid = criaDocumento($cpmid);
$esdid = pegaEstadoAtual($docid);

$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

$abacod_tela = 57568;
$arMnuid = array();

if($esdid != WF_MDS_EM_ANALISE && ( in_array(EQUIPE_MUNICIPAL,$perfis) || in_array(SECRETARIO_ESTADUAL,$perfis))){
	$arMnuid[] = 11127;
}

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$linha1 = "	Lista de Estabelecimentos";
$linha2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';

monta_titulo($linha1, $linha2);

cabecalhoMunicipio();

if($_POST){
	extract($_POST);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('#btnSalvar').click(function(){
			/*var qtds = $('[name=qtd_alunos[]]').val();

			var arQtdsParcial = new Array();
			$.each($('[name=mdsquantidadepbfparcial_temp[]]'), function(i, v){
				arQtdsParcial.push(v.value);
			});

			var arQtdsIntegral = new Array();
			$.each($('[name=mdsquantidadepbfintegral_temp[]]'), function(i, v){
				alert(i+' - '+v);
				//prcqtdalunoinfantilintegral
				arQtdsIntegral.push(v.value);
			});

			var arEntcodent = new Array();
			$.each($('[name=entcodent_temp[]]'), function(i, v){
				arEntcodent.push(v.value);
			});
			var arPrcid = new Array();
			$.each($('[name=prcid_temp[]]'), function(i, v){
				arPrcid.push(v.value);
			});

			var arPrcqtdalunoinfantilintegral = new Array();
			$.each($('[name=prcqtdalunoinfantilintegral[]]'), function(i, v){
				arPrcqtdalunoinfantilintegral.push(v.value);
			});
			
			var arPrcqtdalunoinfantilparcial = new Array();
			$.each($('[name=prcqtdalunoinfantilparcial[]]'), function(i, v){
				arPrcqtdalunoinfantilparcial.push(v.value);
			});
			
			$('[name=entcodent]').val(arEntcodent);
			$('[name=prcid]').val(arPrcid);
			$('[name=mdsquantidadepbfparcial]').val(arQtdsParcial);
			$('[name=mdsquantidadepbfintegral]').val(arQtdsIntegral);
			$('[name=prcqtdalunoinfantilintegral]').val(arPrcqtdalunoinfantilintegral);
			$('[name=prcqtdalunoinfantilparcial]').val(arPrcqtdalunoinfantilparcial);*/
			$('[name=requisicao]').val('salvarQuantidades');
			$('#formAlunosAtendidos').submit();
		});
		$('#btnLimparTudo').click(function(){
			$('.normal').val('');
		});

		$('#btnPesquisar').click(function(){
			
			if($('[name=entnome]').val() == '' && $('[name=entcodent_search]').val() == '' ){
				alert('Selecione pelo menos um filtro!')
				$('[name=entnome]').focus();
				return false;
			}					
			$('#formulario').submit();
		
		});

		$('#btnLimpar').click(function(){
			document.location.href = 'proinfantil.php?modulo=suplementacaomds/listaCreche&acao=A';
		});

		$('.naoconferido').parent().parent().css('background', '#8DAEBA');

		$('#btnVoltar').click(function(){
			document.location.href = 'proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A';
		});
	});
</script>

<form name="formulario" id="formulario" method="post">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">	
		<tr>
			<td class="subtituloDireita">Escola</td>
			<td>
			<?php echo campo_texto('entnome', 'N', 'S', '', 40, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Código INEP</td>
			<td>
			<?php echo campo_texto('entcodent_search', 'N', 'S', '', 15, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Mostrar todos" id="btnLimpar" />
			</td>
		</tr>
		<?php $just = recuperaDiligencia($_SESSION['proinfantil']['mds']['muncod']);
		if(!empty($just)){ ?>
		<tr>
			<td class="subtituloDireita">Diligência:</td>
			<td>
			<?php echo campo_textarea( 'just', 'N', $acesso['estab'], '', 120, 5, '', '', '', '', '', '', $just); ?>
			</td>
		</tr>
		<?php } ?>
	</table>
</form>	
<?php 
monta_titulo($linha1, '');
if($entnome){
	$arWhere[] = " ent.entnome ilike '%{$entnome}%' ";	
}

if($entcodent_search){
	$arWhere[] = " ent.entcodent ilike '%{$entcodent_search}%' ";
}

$sql = "SELECT	 	DISTINCT ent.entcodent,
		            ent.entnome,
		            coalesce(pro.prcqtdalunoinfantilintegral,0) as prcqtdalunoinfantilintegral,
		            coalesce(sup.mdsquantidadepbfintegral,0) as mdsquantidadepbfintegral,
		            sup.mdsconferido,
		            doc.esdid,
		            pro.prcid,
		            coalesce(sup.mdsid,0) as mdsid,
		            coalesce(pro.prcqtdalunoinfantilparcial,0) as prcqtdalunoinfantilparcial,
		            coalesce(sup.mdsquantidadepbfparcial,0) as mdsquantidadepbfparcial
        FROM        entidade.entidade ent
        INNER JOIN	entidade.endereco ede on ede.entid = ent.entid
        INNER JOIN  territorios.municipio mun on mun.muncod = ede.muncod
        INNER JOIN 	proinfantil.mdsdadoscriancapormunicipio dcm on dcm.muncod = ede.muncod and dcm.cpmano = ({$_SESSION['exercicio']} - 1)
        LEFT JOIN 	workflow.documento doc on doc.docid = dcm.docid
        INNER JOIN 	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
        /*LEFT JOIN        	
        			(select * from proinfantil.mdssuplementacao where mdsid in ( 
        				(select max(mdsid) from proinfantil.mdssuplementacao where mdsstatus = 'A' group by entcodent) ) 
        			) as sup on sup.entcodent = ent.entcodent --and sup.prcid = pro.prcid*/
        left join 	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'
		WHERE		ent.entstatus = 'A'
		AND			ent.entcodent is not null						
		AND			mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
					".(is_array($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."
		ORDER BY 	ent.entnome";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

/*

* '
            <input type=\"text\" name=\"mdsquantidadepbfintegral_temp[]\" size=\"8\" class=\"normal ' || 
            		case when sup.mdsconferido = 'f' and prcqtdalunoinfantilintegral > 0 and doc.esdid = ".WF_MDS_EM_DIGILENCIA." then 'naoconferido' else ' ' end || '\" 
            		onKeyUp=\"this.value=mascaraglobal(\'######\',this.value);\" value=\"' || coalesce(mdsquantidadepbfintegral,0) || '\" ' || 
            		case when coalesce(pro.prcqtdalunoinfantilintegral,0) = 0 and (mdsquantidadepbfintegral is null or mdsquantidadepbfintegral = 0) then 'disabled=\"disabled\"' else ' ' end || '/>
            <input type=\"hidden\" name=\"entcodent_temp[]\" value=\"' || ent.entcodent || '\" />
            <input type=\"hidden\" name=\"prcid_temp[]\" value=\"' || pro.prcid || '\" />
            <input type=\"hidden\" name=\"prcqtdalunoinfantilintegral[]\" value=\"' || coalesce(pro.prcqtdalunoinfantilintegral,0) || '\" />
            ' as campo_integral,
            coalesce(pro.prcqtdalunoinfantilparcial,0) as prcqtdalunoinfantilparcial,
            '
            <input type=\"text\" name=\"mdsquantidadepbfparcial_temp[]\" size=\"8\" class=\"normal ' || 
            	case when sup.mdsconferido = 'f' and prcqtdalunoinfantilparcial > 0 and doc.esdid = ".WF_MDS_EM_DIGILENCIA." then 'naoconferido' else ' ' end || '\" 
            		onKeyUp=\"this.value=mascaraglobal(\'######\',this.value);\" value=\"' || coalesce(mdsquantidadepbfparcial,0) || '\" ' || 
            	case when coalesce(pro.prcqtdalunoinfantilparcial,0) = 0 and (mdsquantidadepbfparcial is null or mdsquantidadepbfparcial = 0) then 'disabled=\"disabled\"' else ' ' end || ' />
            	
            <input type=\"hidden\" name=\"prcqtdalunoinfantilparcial[]\" value=\"' || coalesce(pro.prcqtdalunoinfantilparcial,0) || '\" />
            ' as campo_parcial
* */

$cabecalho = array('Cod. INEP', 'Nome da Escola', 'Qtd. de crianças na Creche Integral', 'Creche Integral <br> Qtd. Crianças de 0 a 48 meses - Bolsa Família', 'Qtd. de crianças na Creche Parcial', 'Creche Parcial <br> Qtd. Crianças de 0 a 48 meses - Bolsa Família');
//$db->monta_lista($sql, $cabecalho, 50, 10, 'N','', '', '', array('80','','120','120','120','120'), array('','','center','center','center','center'));
?>
<form name="formAlunosAtendidos" id="formAlunosAtendidos" method="post">
	<input type="hidden" name="requisicao" />
	<!--<input type="hidden" name="mdsquantidadepbfparcial" />
	<input type="hidden" name="mdsquantidadepbfintegral" />
	<input type="hidden" name="prcqtdalunoinfantilintegral" />
	<input type="hidden" name="prcqtdalunoinfantilparcial" />	
	<input type="hidden" name="entcodent" />
	<input type="hidden" name="prcid" /> -->
	<?
	
	$arrRegistro = array();
	foreach ($arrDados as $key => $v) {
		
		$class = '';
		if( $v['sup.mdsconferido'] == 'f' && $v['prcqtdalunoinfantilintegral'] > 0 && $v['esdid'] == WF_MDS_EM_DIGILENCIA ){
			$class = 'naoconferido';
		}
		
		$campoTextoIntegral = '<center><input type="text" style="text-align:right;" id="mdsquantidadepbfintegral_'.$v['entcodent'].'"  name="mdsquantidadepbfintegral['.$v['entcodent'].']" 
							size="10" maxlength="10" value="'.$v['mdsquantidadepbfintegral'].'" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" 
							onKeyUp="this.value=mascaraglobal(\'######\',this.value); verificaQTDIntegral();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" title="" class="normal '.$class.'"></center>';
		
		$campoTextoParcial = '<center><input type="text" style="text-align:right;" id="mdsquantidadepbfparcial_'.$v['entcodent'].'" name="mdsquantidadepbfparcial['.$v['entcodent'].']" 
							size="10" maxlength="10" value="'.$v['mdsquantidadepbfparcial'].'" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" 
							onKeyUp="this.value=mascaraglobal(\'######\',this.value); verificaQTDParcial();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" title="" class="normal '.$class.'"></center>';
		
		$prcqtdalunoinfantilintegral = ($v['prcqtdalunoinfantilintegral'] ? $v['prcqtdalunoinfantilintegral'] : '0');
		$prcqtdalunoinfantilparcial = ($v['prcqtdalunoinfantilparcial'] ? $v['prcqtdalunoinfantilparcial'] : '0');
		
		if( $_SESSION['usucpf'] == '' ){
			$acao = "<img src='../imagens/excluir.gif' style='cursor:pointer;' title=\"Excluir\" onclick=\"if (confirm('Tem certeza que deseja exluir este item?')) window.location.href = 'proinfantil.php?modulo=suplementacaomds/listaCreche&acao=A&mdsiddelete={$v['mdsid']}';\"> ";
		}
		
		$arrRegistro[$key]= array(
							'codinep' => '<center style="color:#0066cc;">'.$v['entcodent'].'</center>
											<input type="hidden" name="prcid['.$v['entcodent'].']" value="'.$v['prcid'].'" />
											<input type="hidden" name="mdsid['.$v['entcodent'].']" value="'.$v['mdsid'].'" />
											<input type="hidden" name="entcodent[]" value="'.$v['entcodent'].'" />'.'</center>',
							'escola' => $acao.$v['entnome'],
							'qtdcriancaintegral' => '<center style="color:#0066cc;">'.$prcqtdalunoinfantilintegral.'</center><input type="hidden" name="prcqtdalunoinfantilintegral['.$v['entcodent'].']" id="prcqtdalunoinfantilintegral_'.$v['entcodent'].'" value="'.$prcqtdalunoinfantilintegral.'" />',
							'creintqtd0a48' => $campoTextoIntegral,
							'qtdcriancaparcial' => '<center style="color:#0066cc;">'.$prcqtdalunoinfantilparcial.'</center><input type="hidden" name="prcqtdalunoinfantilparcial['.$v['entcodent'].']" id="prcqtdalunoinfantilparcial_'.$v['entcodent'].'" value="'.$prcqtdalunoinfantilparcial.'" />',
							'creparcqtd0a48' => $campoTextoParcial,
						);
	}
	
	$db->monta_lista_simples($arrRegistro, $cabecalho, 1000000, 10, 'N', '95%', 'N', true, false, false, true );
	/*$arrParametro = array(
						'sql' 		=> $sql,
						'cabecalho' => $cabecalho,
						'width' => '95%'
						);
	montaTabelaMontaLista($arrParametro);*/ ?>
</form>

<script type="text/javascript">
$(document).ready(function(){
	
	$('[name="entcodent[]"]').each(function(){
		var entcodent = $(this).val();
		
		if( $('#prcqtdalunoinfantilintegral_'+entcodent).val() == '0' ){
			$('#mdsquantidadepbfintegral_'+entcodent).attr('readonly', true);
			$('#mdsquantidadepbfintegral_'+entcodent).removeClass('normal');
			$('#mdsquantidadepbfintegral_'+entcodent).val('0');
		}
		
		if( $('#prcqtdalunoinfantilparcial_'+entcodent).val() == '0' ){
			$('#mdsquantidadepbfparcial_'+entcodent).attr('readonly', true);
			$('#mdsquantidadepbfparcial_'+entcodent).removeClass('normal');
			$('#mdsquantidadepbfparcial_'+entcodent).val('0');
		}
	});
});

function verificaQTDIntegral(){
	
	var entcodent = '';
	var prcqtdalunoinfantilintegral = '';
	var qtdDigitada = '';
	
	$('[name="entcodent[]"]').each(function(){
		entcodent = $(this).val();
		prcqtdalunoinfantilintegral = $('#prcqtdalunoinfantilintegral_'+entcodent).val();
		qtdDigitada = $('#mdsquantidadepbfintegral_'+entcodent).val();
		
		if( parseInt(qtdDigitada) > parseInt(prcqtdalunoinfantilintegral) ){
			alert('A "Qtd. de crianças na Creche Integral" não pode ser menor que a "Qtd. Crianças de 0 a 48 meses - Bolsa Família Integral"');
			$('#mdsquantidadepbfintegral_'+entcodent).val('')
		}
	});
}

function verificaQTDParcial(entcodent){
	
	var entcodent = '';
	var prcqtdalunoinfantilparcial = '';
	var qtdDigitada = '';
	
	$('[name="entcodent[]"]').each(function(){
		entcodent = $(this).val();
		prcqtdalunoinfantilparcial = $('#prcqtdalunoinfantilparcial_'+entcodent).val();
		qtdDigitada = $('#mdsquantidadepbfparcial_'+entcodent).val();
		
		if( parseInt(qtdDigitada) > parseInt(prcqtdalunoinfantilparcial) ){
			alert('A "Qtd. de crianças na Creche Parcial" não pode ser menor que a "Qtd. Crianças de 0 a 48 meses - Bolsa Família Parcial"');
			$('#mdsquantidadepbfparcial_'+entcodent).val('')
		}
	});
}

<?php if($esdid != WF_MDS_EM_CADASTRAMENTO){ ?>
	$(function(){
		$('textarea').attr('disabled', true);
		$('#btnPesquisar, #btnLimpar, [name=entcodent_search], [name=entnome]').attr('disabled', false);
	});	
<?php } ?>

<?php if($esdid == WF_MDS_EM_DIGILENCIA){ ?>
	$(function(){
		$('.naoconferido').attr('disabled', false);
		if($('.naoconferido').length > 0){
			$('#btnSalvar, [name=requisicao], [name=entcodent], [name=prcid] [name=mdsquantidadepbfparcial], [name=mdsquantidadepbfintegral], [name=prcqtdalunoinfantilintegral], [name=prcqtdalunoinfantilparcial]').attr('disabled', false);
		}
	});
<?php } ?>

$(function(){ $('#btnVoltar').attr('disabled', false); });

<?php if($acesso['estab'] == 'N'){ ?>
	$('input').attr('disabled', true);
	$('[name="entnome"]').attr('disabled', false);
	$('[name="entcodent_search"]').attr('disabled', false);
	$('#btnPesquisar').attr('disabled', false);
	$('#btnLimpar').attr('disabled', false);
<?php } ?>
</script>	

<?php if(checkPerfil(array(PERFIL_ADMINISTRADOR,EQUIPE_MUNICIPAL,SECRETARIO_ESTADUAL, PERFIL_SUPER_USUARIO)) && ($esdid == WF_MDS_EM_CADASTRAMENTO || $esdid == WF_MDS_EM_DIGILENCIA)){?>
	<?php if($_SESSION['data_hora_atual'] >= $_SESSION['data_hora_final'] && $esdid != WF_MDS_EM_DIGILENCIA){ ?>
	<script>
		$(function(){
			$('input, textarea').attr('disabled', true);
			$('#btnVoltar').attr('disabled', false);
		});	
	</script>	
	<?php } else { ?>
	<center>
		<p>
			<input type="button" value="Salvar" id="btnSalvar">
			<input type="button" value="Limpar tudo" id="btnLimparTudo">
		</p>
	</center>
	<?php }
} ?>
