<!-- <script src="../library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css" type="text/javascript" charset="ISO-8895-1"></script> -->
<script type="text/javascript">
jQuery(document).ready(function(){
    
	jQuery('[name*=oprid]').attr('disabled',true);
	jQuery('.salvar').attr('disabled',true);
	jQuery('.fechar').attr('disabled',true);
    
    <?php if(in_array(PERFIL_MINC_CADASTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){ ?>
    	jQuery('[name*=oprid]').attr('disabled',false);
    	jQuery('.salvar').attr('disabled',false);
    	jQuery('.fechar').attr('disabled',false);
    <?php } ?>
        
});
jQuery(document).ready(function () {
	if('<?=$queid[3]['bol']?>'!='t'){
		jQuery('.envolvimentodocentes[value="2"]').attr('checked','checked');
	}else{
		jQuery('#tr_envolvimentodocentesqtd').show();
		jQuery('#tr_envolvimentodocentes').show();
		if(jQuery('.questao1:checked').length>=3){
			jQuery('.questao1').attr('disabled','disabled');
			jQuery('.questao1:checked').attr('disabled','');
		}else{
			jQuery('.questao1').attr('disabled','');
		}
	}
	
	
	if(jQuery('.questao11:checked').length>=2){
		jQuery('.questao11').attr('disabled','disabled');
		jQuery('.questao11:checked').attr('disabled','');
	}else{
		jQuery('.questao11').attr('disabled','');
	}
	
	if(jQuery('.questao12:checked').length>=2){
		jQuery('.questao12').attr('disabled','disabled');
		jQuery('.questao12:checked').attr('disabled','');
	}else{
		jQuery('.questao12').attr('disabled','');
	}
	
	jQuery('.questao1').click(function(){
		if(jQuery('.questao1:checked').length>=3){
			jQuery('.questao1').attr('disabled','disabled');
			jQuery('.questao1:checked').attr('disabled','');
		}else{
			jQuery('.questao1').attr('disabled','');
		}
	});
	jQuery('.envolvimentodocentes').click(function(){
		if(jQuery('.envolvimentodocentes:checked').val()=='1'){
	  		jQuery('#tr_envolvimentodocentesqtd').show();
			jQuery('#tr_envolvimentodocentes').show();
		}else{
			jQuery('#qtddocentes').val('');
			jQuery('.questao2').attr('checked','');
	    	jQuery('#tr_envolvimentodocentesqtd').hide();
			jQuery('#tr_envolvimentodocentes').hide();
		}
	});
	jQuery('.colaboraintegracao').click(function(){
		if(jQuery('.colaboraintegracao:checked').val()=='1'){
	  		jQuery('#tr_colaboraintegracao').show();
		}else{
			jQuery('.questao7').attr('checked','');
	    	jQuery('#tr_colaboraintegracao').hide();
		}
	});
	jQuery('.questao11').click(function(){
		if(jQuery('.questao11:checked').length>=2){
			jQuery('.questao11').attr('disabled','disabled');
			jQuery('.questao11:checked').attr('disabled','');
		}else{
			jQuery('.questao11').attr('disabled','');
		}
	});
	jQuery('.questao12').click(function(){
		if(jQuery('.questao12:checked').length>=2){
			jQuery('.questao12').attr('disabled','disabled');
			jQuery('.questao12:checked').attr('disabled','');
		}else{
			jQuery('.questao12').attr('disabled','');
		}
	});
	jQuery('.praticasculturais').click(function(){
		if(jQuery('.praticasculturais:checked').val()=='1'){
	  		jQuery('#tr_praticasculturais').show();
		}else{
			jQuery('.questao8').attr('checked','');
	    	jQuery('#tr_praticasculturais').hide();
		}
	});
	jQuery('.selecaopublico').click(function(){
		if(jQuery('.selecaopublico:checked').val()=='1'){
	  		jQuery('#tr_selecaopublico').show();
		}else{
// 			jQuery('.questao13').attr('checked','');
	    	jQuery('#tr_selecaopublico').hide();
		}
	});
	jQuery('.articuladooutrosprojetos').click(function(){
		if(jQuery('.articuladooutrosprojetos:checked').val()=='1'){
	  		jQuery('#tr_articuladooutrosprojetos').show();
		}else{
			jQuery('.questao14').attr('checked','');
	    	jQuery('#tr_articuladooutrosprojetos').hide();
		}
	});
	jQuery('.utilizouespacovirtual').click(function(){
		if(jQuery('.utilizouespacovirtual:checked').val()=='1'){
	  		jQuery('#tr_utilizouespacovirtual').show();
		}else{
//			jQuery('.questao16').attr('checked','');
	    	jQuery('#tr_utilizouespacovirtual').hide();
		}
	});
	jQuery('.comunidade').blur(function () {
		var soma = 0;
		jQuery('.comunidade').each(function() {
			var qtd = 0;
			if( jQuery(this).val().length !=0 ){
				qtd = jQuery(this).val();
			}
			soma = parseFloat(soma)+parseFloat(qtd);
		});
        jQuery('[id="totalcomunidade"]').html(soma);

    });
	jQuery('.testavalor').change(function(){
		var leaid = jQuery(this).attr('idlista');
		var qtdCenso = jQuery('[id="qtdmatriculados['+leaid+']"]').val();	
		var qtdAlunoMat = jQuery(this).val();
		
		if(parseInt(qtdAlunoMat) > parseInt(qtdCenso) ){
			alert('A quantidade de alunos atendidos não pode ser maior que o número de estudantes matriculados.');
			jQuery(this).val(qtdCenso);
			return false;
		}
	});
	jQuery('.salvar').click(function(){
		jQuery('#requisicao').val('salvarmonitoramento_2');
        jQuery('#form').submit();		
	});
	jQuery('.fechar').click(function(){
		if(jQuery('.questao1:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 1.');
			return false;
		}
		if(jQuery('.envolvimentodocentes:checked').val() == ''){
			alert('Escolha uma opção para a Questão 2.');
			return false;
		}
		if(jQuery('.envolvimentodocentes:checked').val() == '1'){
			if(jQuery('#qtddocentes').val==''){
				alert('Preencha o campo referente a quantidade de docentes.');
				return false;
			}
			if(jQuery('.questao2:checked').length < 1){
				alert('Escolha ao menos uma opção para a Questão 2.');
				return false;
			}
		}
		if(jQuery('.questao3:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 3.');
			return false;
		}
		if(jQuery('.questao3:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 3.');
			return false;
		}
		if(jQuery('.questao4:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 4.');
			return false;
		}
		if(jQuery('.questao5:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 5.');
			return false;
		}
		if(jQuery('.questao6:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 6.');
			return false;
		}
		if(jQuery('.colaboraintegracao:checked').val()=='1'){
			if(jQuery('.questao7:checked').length < 1){
				alert('Escolha ao menos uma opção para a Questão 7.');
				return false;
			}
		}
		if(jQuery('.praticasculturais:checked').val()=='1'){
			if(jQuery('.questao8:checked').length < 1){
				alert('Escolha ao menos uma opção para a Questão 8.');
				return false;
			}
		}
		if(jQuery('.questao10:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 10.');
			return false;
		}
		if(jQuery('.questao11:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 11.');
			return false;
		}
		if(jQuery('.questao12:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 12.');
			return false;
		}
		if(jQuery('.selecaopublico:checked').val()=='1'){
			if(jQuery('#publico').val ==''){
				alert('Escolha ao menos uma opção para a Questão 13.');
				return false;
			}
		}
		if(jQuery('.articuladooutrosprojetos:checked').val()=='1'){
			if(jQuery('.questao14:checked').length < 1){
				alert('Escolha ao menos uma opção para a Questão 14.');
				return false;
			}
		}
		if(jQuery('.questao15:checked').length < 1){
			alert('Escolha ao menos uma opção para a Questão 15.');
			return false;
		}
		if(jQuery('.utilizouespacovirtual:checked').val()=='1'){
			if(jQuery('#link_1').val()=='' && jQuery('#link_2').val()=='' && jQuery('#link_3').val()==''){
				alert('Coloque ao menos um link para a questão 16.');
				return false;
			}	
		}
		if(jQuery('#arquivo1').val()=='' && jQuery('#arquivo2').val()=='' && jQuery('#arquivo3').val()==''){
			alert('Coloque ao menos um Arquivo para a questão 17.');
			return false;
		}
	});
});
function excluirAnexo(arqid) {
	if (confirm('Deseja remover o arquivo?')) {
		jQuery('#arquivo').val(arqid);
		jQuery('#requisicao').val('deletarArquivo');
		jQuery('#form').submit();
	}
}   
    
</script>