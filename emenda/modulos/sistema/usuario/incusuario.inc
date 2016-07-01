<?php

	include APPRAIZ . 'www/emenda/_constantes.php';

	$pflcod = $_REQUEST['pflcod'];
	$usucpf = $_REQUEST['usucpf'];
	
?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php
			$sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A' and sisid=". $sisid ." and pflcod in (".INSTITUICAO_BENEFICIADA.",".AUTOR_EMENDA.") order by descricao";
			//(274,295)
			// and seguranca.perfil.pflnivel > 1
			//$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", 'selecionar_perfil', '', '', '', 'S' );
			$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", 'alterarTextoObservacao', '', '', '', 'S' );
		?>
		<?php //include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
	</td>
</tr>
<?php
	/*$sql = sprintf(
		"select distinct * from emenda.tiporesponsabilidade tr inner join emenda.tprperfil tp on tr.tprcod = tp.tprcod where tr.tprsigla in ('E','A','D') and tp.pflcod = %d and tr.tprsnvisivelperfil = 't'",
		$pflcod
	);
	$responsabilidades = $db->carregar( $sql );*/
	$responsabilidades = 0;
?>
<?php if ( $responsabilidades ): ?>
	<?php foreach( $responsabilidades as $responsabilidade ): ?>
		<tr>
			<td align="right" class="subtitulodireita"><?= $responsabilidade['tprdsc'] ?>:</td>
			<td>
				<input type="hidden" name="perfil" value="<?= $responsabilidade['tprsigla'] ?>"/>
				<?php
					switch ( $responsabilidade['tprsigla'] ) {
						case 'E':
							/*$sql = "SELECT DISTINCT 
									  e.entid as codigo,
									  e.entnumcpfcnpj || ' - ' || e.entnome as descricao 
									FROM 
									  emenda.usuarioresponsabilidade ur	
									INNER JOIN entidade.entidade e 
									       ON (ur.entid = e.entid)
									WHERE 
									  ur.rpustatus='A' 
									  AND ur.usucpf = '$usucpf'";*/
							break;
						case 'A':
							break;
						case 'D':
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
			case 'E':
				break;
			case 'A':
				break;
			case 'D':
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

	function alterarTextoObservacao(value) {
		var textoObservacao = document.getElementById("texto_observacao");
		
		if(value == "") // selecione...
			textoObservacao.innerHTML = "Observações:";
		if(value == 295) //autor
			textoObservacao.innerHTML = "Informe o nome do(s) Autor(es):";
		if(value == 274) // entidade beneficiada
			textoObservacao.innerHTML = "Informe o CNPJ e o nome das entidades:";
		if(value == 310) // analisador
			textoObservacao.innerHTML = "Informe o nome das Unidades:";
	}
	
</script>