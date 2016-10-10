<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Elaboração e Revisão dos Programas e Ações do Plano Pluri-Anual
	 * Finalidade: Controla as especificidades do cadastro de usuários do Sistema de Elaboração e Revisão dos Programas e Ações do Plano Pluri-Anual.
	 * Data de criação:
	 * Última modificação: 04/09/2006
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
		"select distinct * from elabrev.tiporesponsabilidade tr inner join elabrev.tprperfil tp on tr.tprcod = tp.tprcod where tp.pflcod = %d and tr.tprsnvisivelperfil = 't'",
		$pflcod
	);
	$responsabilidades = $db->carregar( $sql );
?>
<?php if ( $responsabilidades ): ?>
	<?php foreach( $responsabilidades as $responsabilidade ): ?>
		<tr>
			<td align="right" class="subtitulodireita"><?= $responsabilidade['tprdsc'] ?>:</td>
			<td>
				<input type="hidden" name="perfil" value="<?= $responsabilidade['tprsigla'] ?>"/>
				<?php
					$sql = null;
					switch ( $responsabilidade['tprsigla'] ) {
						case 'A':
							/*
							$sql = sprintf(
								"select distinct
									pap.acacod,
									a.prgcod||'.'||pap.acacod||'.'||pap.unicod||' - '||a.acadsc as dscacao
								from elabrev.progacaoproposto as pap
									inner join elabrev.acao as a on pap.acacod = a.acacod and pap.prgid = a.prgid and pap.unicod = a.unicod
								where
									a.acasnrap = false and
									pap.usucpf =  '%s'",
								$cpf
							);
							*/
							$sql = sprintf(
								"select distinct
									pap.acacod,
									a.prgcod||'.'||pap.acacod||'.'||pap.unicod||' - '||a.acadsc as dscacao
								from elabrev.progacaoproposto as pap
									inner join elabrev.ppaacao_proposta as a on pap.acacod = a.acacod and pap.prgid = a.prgid and pap.unicod = a.unicod
								where
									a.acastatus = 'A' AND
									pap.usucpf =  '%s'",
								$cpf
							);
							break;
						case 'P':
							$sql = sprintf(
								"select
									pap.prgid,
									p.prgcod||' - '||p.prgdsc as dscprg
								from elabrev.progacaoproposto as pap
									left join elabrev.ppaprograma_proposta as p on pap.prgid = p.prgid
								where
									p.prgstatus = 'A' AND
									pap.usucpf = '%s'",
								$cpf
							);
							break;
						case 'U':
						case 'E':
						case 'N':
						default:
							break;
					}
					if ( $sql ) {
						$opcoes = $db->carregar( $sql );
					}
				?>
				<?php if ( !is_array( $opcoes ) OR $_SESSION['sisid'] == 4 ): ?>
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
								print '<option value='. $prgid .'>'. $dscprg .'</option>';
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
		switch ( tipo ) {
			case 'A':
				especifica_perfil = window.open( "../elabrev/geral/seleciona_aca.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'P':
				especifica_perfil = window.open( "../elabrev/geral/seleciona_prg.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'U':
				especifica_perfil = window.open( "../elabrev/geral/seleciona_unid.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'E':
			case 'N':
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





























<?php
return;
	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Elaboração e Revisão dos Programas e Ações do Plano Pluri-Anual
	 * Finalidade: Controla as especificidades do cadastro de usuários do Sistema de Elaboração e Revisão dos Programas e Ações do Plano Pluri-Anual.
	 * Data de criação:
	 * Última modificação: 04/09/2006
	 */

	$pflcod = $_REQUEST['pflcod'];

?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php
			$sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A' and seguranca.perfil.pflnivel > 1 and sisid=". $sisid ." order by descricao";
			$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", 'selecionar_perfil', '' );
		?>
	</td>
</tr>
<?php if ( $pflcod ) : ?>
	<tr>
		<td align="right" class="subtitulodireita">Unidade proposta(s):</td>
		<td>
			<?php
				$sql = sprintf(
					"SELECT
						unicod as codigo,
						unicod||' - '||unidsc as descricao
					FROM unidade
					WHERE
						unitpocod = 'U' AND
						orgcod = '". CODIGO_ORGAO_SISTEMA. "' AND
						unicod <> '26100' AND
						unistatus = 'A'
					ORDER BY unicod"
				);
				$db->monta_combo_multiplo( "unicod", $sql,  'S', "", '', '', '', 15 );
			?>
		</td>
	</tr>
<?php endif; ?>
<script type="text/javascript">

	function selecionar_perfil(){
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

</script>