<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores SIG
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Monitoramento e Avaliação
	 * Finalidade: Controla as especificidades do cadastro de usuários do Sistema de Monitoramento e Avaliação.
	 * Data de criação:
	 * Última modificação: 05/09/2006
	 */

	$pflcod = $_REQUEST['pflcod'];

?>

<div class="row form-group">
	<label for="regcod" class="col-md-2 control-label">Perfil Desejado</label>
	<div class="col-md-10">
		<?php include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
	</div>
</div>
<script type="text/javascript">
				
	function selecionar_perfil(){
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}
	/**
	 * Recebe os itens selecionados pelo usuário na lista exibida pelo método especificar_perfil()
	 */
	function retorna( objeto, tipo ) {
		campo = document.getElementById( "proposto_"+ tipo );
		tamanho = campo.options.length;
		if ( campo.options[0].value == '' ) {
			tamanho--;
		}
		if ( especifica_perfil.document.formulario.prgid[objeto].checked == true ){
			campo.options[tamanho] = new Option( especifica_perfil.document.formulario.prgdsc[objeto].value, especifica_perfil.document.formulario.prgid[objeto].value, false, false );
		} else {
			for( var i=0; i <= campo.length-1; i++ ) {
				if ( campo.options[i].value == especifica_perfil.document.formulario.prgid[objeto].value ) {
					campo.options[i] = null;
				}
			}
			if ( campo.options[0] ) {
			} else {
				campo.options[0] = new Option( 'Clique Aqui para Selecionar', '', false, false );
			}
		}
	}

</script>