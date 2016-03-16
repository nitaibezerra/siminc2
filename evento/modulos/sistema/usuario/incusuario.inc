<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Monitoramento e Avaliação
	 * Finalidade: Controla as especificidades do cadastro de usuários do Sistema de Monitoramento e Avaliação.
	 * Data de criação:
	 * Última modificação: 05/09/2006
	 */

	$pflcod = $_REQUEST['pflcod'];

?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php
//			$sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A' and seguranca.perfil.pflnivel > 1 and sisid=". $sisid ." order by descricao";
//			$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", 'selecionar_perfil', '', '', '', 'S' );
		?>
		<?php include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
		<!--<select style="width: 200px;" class="CampoEstilo" name="taaid">
			<optgroup label="Eventos">
				<?php
					$sql = sprintf( "select g.gpscod, g.gpsdsc, pf.pflcod, pf.pfldsc from seguranca.perfilgrupo p 
										inner join seguranca.grupoperfilsistema g ON p.gpscod = g.gpscod
										inner join seguranca.perfil pf ON pf.pflcod = p.pflcod
									where g.sisid = 21 and g.gpscod = 1 
									order by pf.pfldsc" );
				?>
				<?php //foreach( $db->carregar( $sql ) as $tipo ): ?>
				<option value="<?= $tipo['pflcod'] ?>"><?= $tipo['pfldsc'] ?></option>
				<?php //endforeach; ?>
			</optgroup>
			<optgroup label="Compras">
				<?php
					$sql = sprintf( "select g.gpscod, g.gpsdsc, pf.pflcod, pf.pfldsc from seguranca.perfilgrupo p 
										inner join seguranca.grupoperfilsistema g ON p.gpscod = g.gpscod
										inner join seguranca.perfil pf ON pf.pflcod = p.pflcod
									where g.sisid = 21 and g.gpscod = 2 
									order by pf.pfldsc" );
				?>
				<?php //foreach( $db->carregar( $sql ) as $tipo ): ?>
				<option value="<?= $tipo['pflcod'] ?>"><?= $tipo['pfldsc'] ?></option>
				<?php //endforeach; ?>
			</optgroup>
		</select>-->
		
	</td>
</tr>
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