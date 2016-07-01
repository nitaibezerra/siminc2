<script>

function carregarListaProfessores(turid) {

	ajaxatualizar('requisicao=carregarListaProfessores&turid='+turid,'modalFormulario');

	jQuery("#modalFormulario").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function validacaoProfessores() {

	jQuery('#formulario').submit();

}

function habilitarValidacao(iusid,obj) {

	if(obj.value=='<?=AED_ANALISE_PARA_CADASTRAMENTO ?>') {
		jQuery('#span_'+iusid).css('display','');
	} else {
		jQuery('#span_'+iusid).css('display','none');
	}
	
}

</script>

<div id="modalFormulario" style="display:none;"></div>

<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="validarCadastramentoTurmas">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	<?
	
	$turma = carregarDadosTurma(array('perfil' => 'coordenadorestadual'));

	$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as cpf,
				   i.iusnome as nome,
				   i.iusemailprincipal as email,
				   CASE WHEN tu.turid IS NULL				    THEN '<b>Não possui turma</b>'
				   		ELSE '<center><img src=\"../imagens/busca.gif\" style=\"cursor:pointer;\" onclick=\"carregarListaProfessores('||tu.turid||');\"></center>' END as lista,
				   CASE WHEN tu.turid IS NULL 					THEN '<span style=\"color:red;\"><b>Não iniciado</b></span>' 
				   		WHEN d.esdid = ".ESD_EM_CADASTRAMENTO." THEN '<span style=\"color:blue;\"><b>Em cadastramento</b></span>'
				   		WHEN d.esdid = ".ESD_EM_ANALISE."       THEN '<span><input type=\"radio\" name=\"tur['||tu.docid||']\" value=\"\" onclick=\"habilitarValidacao('||i.iusid||',this);\" checked> Em análise <input type=\"radio\" name=\"tur['||tu.docid||']\" value=\"".AED_ANALISE_PARA_VALIDACAO."\" onclick=\"habilitarValidacao('||i.iusid||',this);\"> Validar cadastramento <input type=\"radio\" name=\"tur['||tu.docid||']\" value=\"".AED_ANALISE_PARA_CADASTRAMENTO."\" onclick=\"habilitarValidacao('||i.iusid||',this);\"> Retornar para o tutor</span><br><span style=\"display:none;\" id=\"span_'||i.iusid||'\"><textarea id=\"cmddsc\" name=\"cmddsc['||tu.docid||']\" cols=\"70\" rows=\"4\" onmouseover=\"MouseOver( this );\" onfocus=\"MouseClick( this );\" onmouseout=\"MouseOut( this );\" onblur=\"MouseBlur( this );\" style=\"width:70ex;\" class=\"obrigatorio txareanormal\"></textarea></span>'
				   		WHEN d.esdid = ".ESD_VALIDADO."         THEN '<span style=\"color:green;\"><b>Validado</b></span>' END as aprov
			FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_TUTOR." 
			LEFT JOIN escolaterra.turmas tu ON tu.iusid = t.iusid 
			LEFT JOIN workflow.documento d ON d.docid = tu.docid  
			INNER JOIN escolaterra.turmaidusuario u ON u.iusid = i.iusid 
			WHERE u.turid='".$turma['turid']."'";
	
	$cabecalho = array("CPF","Nome","Email","Professores Cadastrados","&nbsp;");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);

	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Anterior" onclick="divCarregando();window.location='escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&aba=cadastro';"> <input type="button" value="Salvar" onclick="validacaoProfessores();"></td>
</tr>
</table>
</form>