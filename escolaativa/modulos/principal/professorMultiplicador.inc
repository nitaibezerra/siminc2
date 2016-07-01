<?php

/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */

require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "includes/classes/entidades.class.inc";
require_once APPRAIZ . "escolaativa/classes/FormacaoProfessor.class.inc";
require_once APPRAIZ . "escolaativa/classes/ExperienciaProfessor.class.inc";
require_once APPRAIZ . "escolaativa/classes/Tecnico.class.inc";
require_once APPRAIZ . "escolaativa/classes/TecnicoResponsabilidade.class.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";

print '<br/>';

if($_REQUEST['opt'] == 'salvarRegistro'){
	//ver($_POST,d);
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	$entid = $entidade->getEntId();

	$tecid = $_POST['tecid'];
	$oTecnico = new Tecnico($tecid);
	if(!$oTecnico->pesquisaTecnicoPorEntid($entid)){
		//$oTecnico->tecid = '';
		$oTecnico->entid = $entid;
		if(!$_SESSION['escolaativa']['boPerfilMultiplicador']){
			if($_SESSION['escolaativa']['muncod']){
				$oTecnico->muncod = $_SESSION['escolaativa']['muncod'];			
			} elseif($_SESSION['escolaativa']['estuf']){
				$oTecnico->estuf = $_SESSION['escolaativa']['estuf'];
			}			
		}
		//$oTecnico->cobid = null;
		$tecid = $oTecnico->salvar();
	}
	
	//ver($tecid, $_POST['tpcidf'], d);
	if($_POST['tpcidf'][0]){
		$oFormacaoProfessor = new FormacaoProfessor();
		$oFormacaoProfessor->deletarFormacaoPorTecid($tecid);
		
		for($x=0;$x<count($_REQUEST['tpcidf']);$x++){
			if(trim($_REQUEST['fopdtinicio'][$x])){
				$dataI = formata_data_sql($_REQUEST['fopdtinicio'][$x]);
			}
			if(trim($_REQUEST['fopdtconclusao'][$x])){
				$dataF = formata_data_sql($_REQUEST['fopdtconclusao'][$x]);				
			}
			$fopinconcluido = $_REQUEST['fopinconcluido'][$x] ? true : 'f';
			$oFormacaoProfessor->fopid	 		= null;
			$oFormacaoProfessor->tecid	 		= $tecid;
			$oFormacaoProfessor->tpcid 			= $_REQUEST['tpcidf'][$x];
			$oFormacaoProfessor->fopinconcluido = $fopinconcluido;			
			$oFormacaoProfessor->fopdtinicio 	= $dataI;
			$oFormacaoProfessor->fopdtconclusao = $dataF;
			$oFormacaoProfessor->fopdsccurso 	= $_REQUEST['fopdsccurso'][$x];
			$oFormacaoProfessor->salvar();
		}	
	}
	
	if($_REQUEST['expcargo'][0]){
		$oExperienciaProfessor = new ExperienciaProfessor();
		$oExperienciaProfessor->deletarExperienciaPorTecid($tecid);
		for($x=0;$x<count($_REQUEST['expcargo']);$x++){
			$dataA = formata_data_sql($_REQUEST['expdtadmissao'][$x]);
			$oExperienciaProfessor->expid 			= null;
			$oExperienciaProfessor->tecid 			= $tecid;
			$oExperienciaProfessor->expinstituicao 	= $_REQUEST['expinstituicao'][$x];
			$oExperienciaProfessor->expcargo 		= $_REQUEST['expcargo'][$x];
			$oExperienciaProfessor->expdtadmissao 	= $dataA;
			$oExperienciaProfessor->salvar();
		}
	}
	
	/*
	 * Excluimos todas as responsabilidade pelo tecid da tabela tecnicoresponsabilidade e gravamos todas as responsabilidade que foram atribuidas no perfil.
	 */
	if($_SESSION['escolaativa']['boPerfilMultiplicador']){
		$sql = "SELECT muncod FROM escolaativa.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' ";
		$arMuncod = $db->carregar($sql);
		$arMuncod = ($arMuncod) ? $arMuncod : array();
		
		$oTecnicoResponsabilidade = new TecnicoResponsabilidade();
		$oTecnicoResponsabilidade->excluiResponsabilidadePorTecid($tecid);
		foreach($arMuncod as $municipio){
			$oTecnicoResponsabilidade->muncod = $municipio['muncod'];
			$oTecnicoResponsabilidade->tecid  = $tecid;
			$oTecnicoResponsabilidade->salvar();
		}
	} elseif($_SESSION['escolaativa']['boPerfilSupervisor']){
		$sql = "SELECT estuf FROM escolaativa.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' ";
		$arEstuf = $db->carregar($sql);
		$arEstuf = ($arEstuf) ? $arEstuf : array();
		
		$oTecnicoResponsabilidade = new TecnicoResponsabilidade();
		$oTecnicoResponsabilidade->excluiResponsabilidadePorTecid($tecid);
		foreach($arEstuf as $estado){
			$oTecnicoResponsabilidade->estuf = $estado['estuf'];
			$oTecnicoResponsabilidade->tecid  = $tecid;
			$oTecnicoResponsabilidade->salvar();
		}
	}
	
	$oTecnico->commit();
	echo "<script>
			alert('Operação realizada com sucesso');
			window.location.href = 'escolaativa.php?modulo=inicio&acao=C';
		  </script>";
	unset($oTecnico);		
	
}

$titulo_modulo = "Escola Ativa";
monta_titulo( $titulo_modulo, 'Cadastro do Bolsista' );


$arSegmentos = array();

$entidade = new Entidades();

$entid = $db->pegaUm("SELECT e.entid FROM entidade.entidade e WHERE e.entnumcpfcnpj = '{$_SESSION['usucpf']}'");
if($entid){
	$entidade->carregarPorEntid($entid);	
}

echo $entidade->formEntidade("?modulo=principal/professorMultiplicador&acao=A&opt=salvarRegistro",
							 array("funid" => FUNCAO_PROFESSOR_MULTIPLICADOR_ESCOLA_ATIVA, "entidassociado" => null),
							 array( "enderecos"=>array(1) ) );
							 
							 
if($_SESSION['escolaativa']['tecid']){
	$tecid = $_SESSION['escolaativa']['tecid']; 
} elseif ($_REQUEST['tecid']) {
	$tecid = $_REQUEST['tecid']; 
}

$obData = new Data();
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
jQuery.noConflict();

jQuery(document).ready(function() {

	jQuery('#tr_endmapa').after('<tr><td colspan=\'2\' >'+jQuery('#divFormacaoExperiencia').html()+'</td></tr>');
	jQuery('#divFormacaoExperiencia').remove();

	jQuery('#frmEntidade').submit(function(){
		var msg = '';
		jQuery('select[name=tpcidf[]]').each(function(i){
			var fopdsccurso    = jQuery(this).parent().parent().find('input[name=fopdsccurso[]]:first');
			var fopdtinicio    = jQuery(this).parent().parent().find('input[name=fopdtinicio[]]:first');
			var fopdtconclusao = jQuery(this).parent().parent().find('input[name=fopdtconclusao[]]:first');

			if(jQuery(this).val() == '' || fopdsccurso.val() == '' || fopdtinicio.val() == '' || fopdtconclusao.val() == ''){
				msg += 'Os campos Tipo, Curso, Data de Início e Data de Fim da linha '+ (i+1) +' são obrigatórios. \n';
			}
		});

		var boQuebra = false;
		jQuery('input[name=expinstituicao[]]').each(function(i){
			var expcargo	  = jQuery(this).parent().parent().find('select[name=expcargo[]]:first');
			var expdtadmissao = jQuery(this).parent().parent().find('input[name=expdtadmissao[]]:first');

			if(msg && !boQuebra){
				msg += '--------------------------------------------------------------------------------------------------------------\n';
				boQuebra = true;
			}
			
			if(jQuery(this).val() == '' || expcargo.val() == '' || expdtadmissao.val() == ''){
				msg += 'Os campos Instituição de atuação, Cargo e Data de atuação '+ (i+1) +' são obrigatórios. \n';
			}
			
		});

		if(msg){
			alert(msg);
			return false;
		}
		
	});
	
	jQuery('#btncancelar').click(function(){
		window.location.href = 'escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A';
	});

	jQuery('.addFormacao').click(function(){
		var $campos = jQuery('#tbodyTabelaFormacao'),
				$tr = $campos.find('tr:first').clone();

				// fopdtinicio				
				var id = 'fopdtinicio_'+jQuery('input[name=fopdtinicio[]]').length;
				$tr.find('input[name=fopdtinicio[]]:first').attr('id',id);

				var img = "<img border=\"0\" align=\"absmiddle\" onclick=\"displayCalendar(document.getElementById('"+id+"'),'dd/mm/yyyy',this)\" title=\"Escolha uma Data\" style=\"cursor: pointer;\" src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\">";
				$tr.find('input[name=fopdtinicio[]]:first').parent().find('img:first').after(img).remove();

				// fopdtconclusao
				var id = 'fopdtconclusao_'+jQuery('input[name=fopdtconclusao[]]').length;
				$tr.find('input[name=fopdtconclusao[]]:first').attr('id',id);

				var img = "<img border=\"0\" align=\"absmiddle\" onclick=\"displayCalendar(document.getElementById('"+id+"'),'dd/mm/yyyy',this)\" title=\"Escolha uma Data\" style=\"cursor: pointer;\" src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\">";
				$tr.find('input[name=fopdtconclusao[]]:first').parent().find('img:first').after(img).remove();

				$tr.find("input").val("");
				$tr.find("input[type=checkbox]").attr('checked',false);
				$tr.find("select").val("");
			$campos.append($tr);
		return false;
	});

	jQuery('.removeItensFormacao').live('click',function(){
		var filhos = jQuery('#tbodyTabelaFormacao').children().length;
		if (filhos > 1) {
			if(confirm("Deseja excluir esta linha?")){
				jQuery(this).parent().parent().remove();
			}
		}
		return false;
	});
	
	jQuery('.addExperiencia').click(function(){
		var $campos = jQuery('#tbodyTabelaExperiencia'),
				$tr = $campos.find('tr:first').clone();
				$tr.find("input").val("");
				$tr.find("select").val("");
				
				// fopdtinicio				
				var id = 'expdtadmissao_'+jQuery('input[name=expdtadmissao[]]').length;
				$tr.find('input[name=expdtadmissao[]]:first').attr('id',id);

				var img = "<img border=\"0\" align=\"absmiddle\" onclick=\"displayCalendar(document.getElementById('"+id+"'),'dd/mm/yyyy',this)\" title=\"Escolha uma Data\" style=\"cursor: pointer;\" src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\">";
				$tr.find('input[name=expdtadmissao[]]:first').parent().find('img:first').after(img).remove();
				
				
			$campos.append($tr);
		return false;
	});

	jQuery('.removeItensExperiencia').live('click',function(){
		var filhos = jQuery('#tbodyTabelaExperiencia').children().length;
		if (filhos > 1) {
			if(confirm("Deseja excluir esta linha?")){
				jQuery(this).parent().parent().remove();
			}
		}
		return false;
	});

	jQuery('.row_days').live('click', function(){
		jQuery('input[name=fopdtinicio[]]').each(function(i){
			var fopdtinicio = jQuery(this);
			var fopdtconclusao = jQuery(this).parent().parent().find('input[name=fopdtconclusao[]]:first');
			if(fopdtinicio.val() && fopdtconclusao.val()){
				var dtInicio    = fopdtinicio.val();
				var dtConclusao = fopdtconclusao.val();
				
				dtInicio    = parseInt( dtInicio.split( "/" )[2].toString() + dtInicio.split( "/" )[1].toString() + dtInicio.split( "/" )[0].toString() );
				dtConclusao = parseInt( dtConclusao.split( "/" )[2].toString() + dtConclusao.split( "/" )[1].toString() + dtConclusao.split( "/" )[0].toString() );
				
				if ( dtInicio > dtConclusao ) {
				    alert('A Data de Conclusão não pode ser maior que a Data de Início.');
				    fopdtconclusao.val('');
				    fopdtconclusao.focus();
					return false;
				}
			}
		});

	});
		
});

//-->
</script>

<?
$texto = "<div style=\"font-size:12px\" >Senhores (as) Usuários (as),<br /><br />
Informamos que os Cadernos de Ensino e Aprendizagem, com adequações, do Programa Escola Ativa, constante deste módulo, encontra-se disponível para download.<br/>
Cadernos de Ensino e Aprendizagem, com adequações, do Programa Escola Ativa: <a target=\"_blank\" href=\"escolaativa.php?modulo=sistema/geral/downloads_usu&acao=A\" >clique aqui</a>.</div>";
popupAlertaGeral($texto,'450px',"250px");
?>
<div id="divFormacaoExperiencia" style="display: none; disabled: " >
<input type='hidden' id='tecid' name='tecid' value='<?php echo $tecid;?>' />
<table id="tabelaEquipeLocal" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" style="width: 100%" >
	<thead>
		<tr align="center" style=" background: #ccc; color: #000;">
			<td colspan="6" style="font-weight: bold; font-size: 10px;">FORMAÇÃO</td>	
		</tr>
		<tr align="center">
			<td width="3%">&nbsp;</td>	
			<td width="25%">Tipo</td>	
			<td width="25%">Curso</td>	
			<td width="15%">Concluído</td>	
			<td width="15%">Data de Início</td>	
			<td width="15%">Data de Fim</td>	
		</tr>	
	</thead>
	<tbody id="tbodyTabelaFormacao">
	<?php
	if($tecid){
		$sql = "SELECT tecid,
				   tpcid,
				   fopid,
				   fopinconcluido,
				   fopdtinicio,
				   fopdtconclusao,
				   fopdsccurso
			FROM escolaativa.formacaoprofessor WHERE tecid = {$tecid}";
		$arFomacaoProfessor = $db->carregar($sql);
	} 
	$arFomacaoProfessor = $arFomacaoProfessor ? $arFomacaoProfessor : array(array(1)); 
	?>
	<?php $i = 0;?>
	<?php foreach ($arFomacaoProfessor as $dados) : ?>
		<?php extract($dados);?>	
		<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" align="center">
			<td>
				<img class="removeItensFormacao" alt="Excluir" title="Excluir" src="/imagens/excluir.gif">
			</td>
			<td>
				<?php 
				  $sql = "SELECT tpcid as codigo, tpcdsc as descricao  FROM public.tipocurso ORDER BY tpcdsc ASC";
        		  $arTipoCurso = $db->carregar($sql);
				?>
				<select class="comboEstilo exibicaoTipoCurso" name="tpcidf[]">
					<option value="">Selecione</option>
					<?php 
						foreach ($arTipoCurso as $tipoCurso) {
							echo "<option value='" . $tipoCurso['codigo'] . "' " . (($tipoCurso['codigo'] == $tpcid) ? "selected=\"selected\"" : "")  . ">" . $tipoCurso['descricao'] . "</option>";
						}
					?>
				</select>
			</td>
			<td>
				<input size="30" style="width: 33ex;" value="<?php echo $fopdsccurso; ?>" name="fopdsccurso[]" type="text">
			</td>
			<td>
				<?php $checked = ($fopinconcluido == 't') ? "checked=\"checked\"" : ""; ?>
				<input type="checkbox" name="fopinconcluido[]" id="fopinconcluido" <?php echo $checked; ?> />
			</td>
			<td>
				<?php
					if($fopdtinicio){
						$fopdtinicio = $obData->formataData($fopdtinicio,"dd/mm/YYYY");						
					}
					echo campo_data2('fopdtinicio[]', 'N', 'S', 'Data', '##/##/####', '', 'VerificaData(this, this.value);', $fopdtinicio, '', '',"fopdtinicio_$i"); 
				?>
			</td>
			<td>
				<?php
					if($fopdtconclusao){
						$fopdtconclusao = $obData->formataData($fopdtconclusao,"dd/mm/YYYY");						
					}
					echo campo_data2('fopdtconclusao[]', 'N', 'S', 'Data', '##/##/####', '', 'VerificaData(this, this.value);', $fopdtconclusao, '', '',"fopdtconclusao_$i"); 
				?>
			</td>
		</tr>
		<?php $i++;?>
	<?php endforeach; ?>
	</tbody>
</table>
<table class="tabela" cellspacing="2" cellpadding="2" border="0" align="center" style="width: 100%" >
	<tr>
		<td colspan="6" class="SubTituloEsquerda" >
			<span class="addFormacao" style="margin-left:5px; cursor:pointer">
				<img src="/imagens/gif_inclui.gif" align="top" style="border: none" />
				Inserir formação
			</span>
		</td>
	</tr>		
</table>

<table id="tabelaEquipeLocal" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" style="width: 100%" >
	<thead>
		<tr align="center" style=" background: #ccc; color: #000;">
			<td colspan="6" style="font-weight: bold; font-size: 10px;">EXPERIÊNCIA</td>	
		</tr>
		<tr align="center">
			<td width="4%">&nbsp;</td>	
			<td width="32%">Instituição de atuação</td>	
			<td width="32%">Cargo</td>	
			<td width="32%">Data de atuação na instituição de ensino</td>	
		</tr>	
	</thead>
	<tbody id="tbodyTabelaExperiencia">
	<?php
		if($tecid){
			$sql = "SELECT 
						expid,
		  				tecid,
		  				expinstituicao,
						expcargo,
						expdtadmissao
					FROM escolaativa.experienciaprofessor WHERE tecid = {$tecid}";
			$arExperienciaProfessor = $db->carregar($sql);
		} 
		$arExperienciaProfessor = $arExperienciaProfessor ? $arExperienciaProfessor : array(array(1));
	?>
	<?php $i = 0;?>
	<?php foreach ($arExperienciaProfessor as $dados) : ?>
		<?php extract($dados);?>	
		<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1" align="center">
			<td>
				<img class="removeItensExperiencia" alt="Excluir" title="Excluir" src="/imagens/excluir.gif">
			</td>
			<td>
				<input size="30" style="width: 33ex;" value="<?php echo $expinstituicao; ?>" name="expinstituicao[]" type="text">
			</td>
			<td>
				<?php 
        		  $sql = "SELECT carid as codigo, cardsc as descricao FROM public.cargo ORDER BY cardsc ASC";
        		  $arCargos = $db->carregar($sql);
				?>
				<select class="comboEstilo exibicaoTipoCurso" name="expcargo[]">
					<option value="">Selecione</option>
					<?php 
						foreach ($arCargos as $cargos) {
							echo "<option value='" . $cargos['codigo'] . "' " . (($cargos['codigo'] == $expcargo) ? "selected=\"selected\"" : "")  . ">" . $cargos['descricao'] . "</option>";
						}
					?>
				</select>
			</td>
			<td>
				<?php
					if($expdtadmissao){
						$expdtadmissao = $obData->formataData($expdtadmissao,"dd/mm/YYYY");						
					}
					echo campo_data2('expdtadmissao[]', 'N', 'S', 'Data', '##/##/####','', 'VerificaData(this, this.value);', $expdtadmissao, '', '',"expdtadmissao_$i"); 
				?>
			</td>
		</tr>
		<?php $i++;?>
	<?php endforeach; ?>
	</tbody>
</table>
<table class="tabela" cellspacing="2" cellpadding="2" border="0" align="center" style="width: 100%" >
	<tr>
		<td class="SubTituloEsquerda" >
			<span class="addExperiencia" style="margin-left:5px; cursor:pointer">
				<img src="/imagens/gif_inclui.gif" align="top" style="border: none" />
				Inserir experiência
			</span>
		</td>
	</tr>		
</table>
</div>
<script type="text/javascript">

if($('entnumcpfcnpj').value){
	document.getElementById('entnumcpfcnpj').readOnly = true;
	document.getElementById('entnumcpfcnpj').className = 'disabled';
	document.getElementById('entnumcpfcnpj').onfocus = "";
	document.getElementById('entnumcpfcnpj').onmouseout = "";
	document.getElementById('entnumcpfcnpj').onblur = "";
	document.getElementById('entnumcpfcnpj').onkeyup = "";	
}

if($('entnome').value){
	document.getElementById('entnome').readOnly = true;
	document.getElementById('entnome').className = 'disabled';
	document.getElementById('entnome').onfocus = "";
	document.getElementById('entnome').onmouseout = "";
	document.getElementById('entnome').onblur = "";
	document.getElementById('entnome').onkeyup = "";	
}

</script>