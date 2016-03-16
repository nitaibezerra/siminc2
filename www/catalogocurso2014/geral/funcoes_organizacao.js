function bloquearCamposClasse( func ){
	jQuery.globalEval("$('#orcdesc')."+func+"('disabled');");
	jQuery.globalEval("$('#pacod_mod_ensino')."+func+"('disabled2');");
	jQuery.globalEval("$('#orcchmim')."+func+"('disabled');");
	jQuery.globalEval("$('#orcchmax')."+func+"('disabled');");
	jQuery.globalEval("$('#orcpercpremim')."+func+"('disabled');");
	jQuery.globalEval("$('#orcpercpremax')."+func+"('disabled');");
	jQuery.globalEval("$('#orcementa')."+func+"('disabled2');");
}

function bloquearCampos(val){modid
	$('#orcdesc').attr('disabled',val);
	$('#modid').attr('disabled',val);
	$('#pacod_mod_ensino').attr('disabled',val);
	$('#orcchmim').attr('disabled',val);
	$('#orcchmax').attr('disabled',val);
	$('#orcpercpremim').attr('disabled',val);
	$('#orcpercpremax').attr('disabled',val);
	$('#orcementa').attr('disabled',val);
}

function excluirOrganizacao( orcid ) {
	
	if( confirm( 'Realmente deseja excluir esta organização?' ) ){
		$('#req').val('excluirOrganizacaoCurso');
		$('#orcid').val(orcid);
		$('#frmOrganizacao').submit();
	}
}