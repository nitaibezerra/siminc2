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
		<?php include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
	</td>
</tr>
<?php
	$sql = sprintf(
		"select distinct * from pdeescola.tiporesponsabilidade tr inner join  pdeescola.tprperfil tp on tr.tprcod = tp.tprcod where tr.tprsigla = 'S' and tp.pflcod = %d and tr.tprsnvisivelperfil = 't'",
		$pflcod
	);
	$responsabilidades = $db->carregar( $sql );
?>
<?php if ( $responsabilidades &&  $pflcod == 383 ): ?>
	<?php foreach( $responsabilidades as $responsabilidade ): ?>
		<tr>
			<td align="right" class="subtitulodireita"><?= $responsabilidade['tprdsc'] ?>:</td>
			<td>
				<input type="hidden" name="perfil" value="<?= $responsabilidade['tprsigla'] ?>"/>
				<?php
					switch ( $responsabilidade['tprsigla'] ) {
						case 'S':
							$sql = sprintf("SELECT DISTINCT
												ent.entid,
												ent.entnome
											FROM
												entidade.entidade ent 
											INNER JOIN 
												entidade.funcaoentidade as fe ON fe.entid = ent.entid
											INNER JOIN 
												entidade.entidadedetalhe ed ON ed.entid = ent.entid AND
											 								   ed.entpdeescola = 't'
											
											LEFT JOIN pdeescola.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
											 												  rpustatus = 'A'	
											WHERE
												fe.funid = 3 and
												ur.usucpf = '%s'	 
											ORDER BY
												ent.entnome ",
												$cpf);
							break;
						case 'E':
							break;
						case 'M':
							break;
						default:
							break;
					}
					if ( $sql ) {
						$opcoes = $db->carregar( $sql );
					}
					
				?>
				<?php if ( !is_array( $opcoes ) OR $_SESSION['sisid'] == 34 ): ?>
					<select multiple size="5" name="proposto[<?= $responsabilidade['tprsigla'] ?>][]" id="proposto_<?= $responsabilidade['tprsigla'] ?>" style="width:500px;" onclick="especificar_perfil( '<?= $responsabilidade['tprsigla'] ?>' )" class="CampoEstilo">
						<option value="">Clique Aqui para Selecionar</option>
					</select>
					<?=obrigatorio();?>
					<br/>
				<?php else: ?>
					<select multiple size="5" name="proposto[<?= $responsabilidade['tprsigla'] ?>][]" id="proposto_<?= $responsabilidade['tprsigla'] ?>" style="width:500px;" onclick="" class="CampoEstilo">
						<?php
							foreach ( $opcoes as $opcao ) {
								extract( $opcao );
								print '<option value='. $entid .'>'. $entnome .'</option>';
							}
						?>
					</select>
					<?=obrigatorio();?>
				<?php endif; ?>
			</td>
		</tr>
	<?php endforeach; ?>
<?php endif; ?>
<script type="text/javascript">
				
	function selecionar_perfil(){
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

	/**
	 * Exibe opções relacionadas ao perfil. (ação, programa e projeto especial )
	 */
	function especificar_perfil( tipo ){
		document.getElementById( "proposto_"+ tipo ).selectedIndex = -1;
		var muncod = document.getElementById( 'muncod' ).value;
		
		switch ( tipo ) {
			case 'S':
				especifica_perfil = window.open( "../pdeescola/geral/seleciona_escola.php?campo=proposto_"+ tipo + "&muncod=" + muncod, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'M':
				break;
			case 'E':
				break;
			defaul:
				break;
		}
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