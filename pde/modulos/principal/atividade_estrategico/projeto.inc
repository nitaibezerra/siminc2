<?php

// captura dados do projeto da requisição
$atiid = $_REQUEST['atiid'] ? (integer) $_REQUEST['atiid'] : PROJETO;

// captura as inforamções da atividade
$atividade = atividade_pegar( $atiid );
if ( !$atividade ) {
	redirecionar( $_SESSION['paginainicial'], null );
}

//$permissao = projeto_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao = true;
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// efetiva a alteração dos dados
if ( $_REQUEST['gravar'] ) {
	$sql = sprintf(
		"update pde.atividade set atidescricao = '%s', atiapelido = '%s', atidetalhamento = '%s', exoid = '%d', atiacaid = '%d', atiexercicio = '%s', atimetaglobal = '%s' where atiid = %d",
		$_REQUEST['atidescricao'],
		$_REQUEST['atiapelido'],
		$_REQUEST['atidetalhamento'],
		$_REQUEST['exoid'],
		$_REQUEST['atiacaid'],
		$_REQUEST['atiexercicio'],
		$_REQUEST['atimetaglobal'],
		$_REQUEST['atiid']
	);
	
		//AGENDA DE GOVERNO
		$sqlA = "DELETE FROM pde.agendaatividade WHERE atiid =  ". $_REQUEST['atiid'] ."";			
		$db->executar($sqlA);
		$_POST['aggid'] = $_POST['aggid'] ? $_POST['aggid'] : array();
		foreach($_POST['aggid'] as $aggid) {
			if($aggid){
				$sqlA = "INSERT INTO pde.agendaatividade(aggid, atiid) VALUES (". $aggid .", ". $_REQUEST['atiid'] .")";			
				$db->executar($sqlA);
			}
		}
	
	if ( $db->executar( $sql ) ) {
		
		$_REQUEST['apoio'] = array_diff( $_REQUEST['apoio'], $_REQUEST['gerente'] ); # impede que os usuários redebam atribuição dupla de responsabilidade
		if ( !empty( $_REQUEST['gerente'] ) ) {
			atividade_atribuir_responsavel( $_REQUEST['atiid'], PERFIL_GESTOR, $_REQUEST['gerente'] );
		}
		atividade_atribuir_responsavel( $_REQUEST['atiid'], PERFIL_EQUIPE_APOIO_GESTOR, $_REQUEST['apoio'] );
		$db->commit();
		$db->sucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
	} else {
		$db->rollback();
		$db->insucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
	}
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

extract( $atividade ); # mantém o formulário preenchido

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
	
	function cancelar() {
		window.location.href = '?modulo=<?= $_SESSION['paginainicial'] ?>';
	}
	
	function enviar(){
		if ( document.formulario.atidescricao.value == '' ) {
			alert( 'Preencha o campo Descrição' );
			document.formulario.atidescricao.focus();
			return;
		}
		selectAllOptions( formulario.gerente );
		selectAllOptions( formulario.apoio );
		selectAllOptions( document.getElementById( 'aggid' ) );
		
		var erro = 0;
			jQuery("[class~=obrigatorio]").each(function() { 
				if(!this.value || this.value == "Selecione..."){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios.');
					this.focus();
					return false;
				}
			});
		if(erro == 0){
			document.formulario.submit();
		}
		
	}
	
	function addExercicioMeta()
	{
		var ano  = jQuery("[name='ano']").val();
		var meta = jQuery("[name='meta']").val();
		
		if(!ano){
			alert('Informe o Exercício.');
			return false;
		}
		
		if(!meta){
			alert('Informe a meta.');
			return false;
		}
		
		if(jQuery("#hdn_exercicio_" + ano).val()){
			alert('Já existe uma meta para ' + ano + '.');
			return false;
		}
		
		jQuery("#tr_botoes_acao").before("<tr><td class='SubtituloDireita' ></td><td><img src='../imagens/excluir.gif' title='Excluir meta' onclick='excluirMetaExercicio(this)' style='cursor:pointer;vertical-align:middle'  /> <input type='hidden' name='hdn_exercicio[]' id='hdn_exercicio_" + ano + "' value='" + ano + "' /><input type='hidden' name='hdn_meta[]' value='" + meta + "' />Meta de " + meta + " para o exercício de " + ano + ".</td></tr>");
		
	}
	
	function excluirMetaExercicio(obj)
	{
		jQuery(obj).parent().parent().remove();
	}

</script>
<form action="" method="post" name="formulario">
	<input type="hidden" name="atiid" value="<?= $atiid ?>"/>
	<input type="hidden" name="gravar" value="1"/>
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Título:</td>
			<td><?= campo_textarea( 'atidescricao', 'S', $permissao_formulario, '', 70, 3, 500 ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Apelido:</td>
			<td>
				<?php echo campo_texto('atiapelido','N',$permissao_formulario,'',25, 20,'','',null,null,null,'id="atiapelido"'); ?>
			</td>
		</tr>		
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
			<td><?= campo_textarea( 'atidetalhamento', 'N', $permissao_formulario, '', 70, 10, '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" >Eixo:</td>
			<td>
				<?php
					$sql= "SELECT 
								exodsc AS descricao, 
								exoid AS codigo
							FROM 
								painel.eixo
							order by
								 descricao";
					$db->monta_combo("exoid",$sql,$permissao_formulario,"Selecione...","","","","200","S","","",$exoid);
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" >Ação:</td>
			<td>
				<?php
					$sql= "select 
								acaid as codigo,
								acadsc as descricao
							from
								painel.acao
							where
								acastatus = 'A'
							order by
								acadsc";
					$acaid = $acaid_indicador;
					$db->monta_combo("atiacaid",$sql,$permissao_formulario,"Selecione...","","","","200","S","","",$atiacaid);
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Agenda(s) de Governo(s):</td>
			<td>
			<?php 
			$Sql = "SELECT 
						aggdsc AS descricao, 
						aggid AS codigo
					FROM 
						painel.agendagoverno
					WHERE 
						aggstatus = 'A'
					order by
						 descricao";
			if($atiid){
				$SqlCarregado = "SELECT 
									aggdsc AS descricao, 
									agi.aggid AS codigo
								FROM 
									painel.agendagoverno age
								INNER JOIN
									pde.agendaatividade agi ON age.aggid = agi.aggid
								WHERE 
									aggstatus = 'A'
								and
									atiid = $atiid
								order by
									 descricao";
				$aggid = $db->carregar($SqlCarregado);
			}
			combo_popup( 'aggid', $Sql, 'Selecione a(s) Agenda(s) de Governo(s)', '360x460', 0, array(), "", $permissao, false, false, 5, 400);
			?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Assessor Responsável:</td>
			<td>
				<?php
					$sql_gerente = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
							inner join pde.usuarioresponsabilidade ur on
								ur.usucpf = u.usucpf
							inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
						where
							ur.rpustatus = 'A' and
							ur.pflcod = '" . PERFIL_GESTOR . "' and
							atiid = " . $atividade['atiid'] . "
						order by u.usucpf
					";
					$gerente = $db->carregar( $sql_gerente );
					$gerente = $gerente ? $gerente : array();
					$sql_combo = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf  
						where
							( u.suscod = 'A' or u.suscod = 'P' ) and
							us.suscod = 'A' and
							us.sisid = ". $_SESSION['sisid'] ."
						group by u.usucpf, u.usunome"; 
					combo_popup(
						'gerente',
						$sql_combo,
						'Selecione o(s) Identificador(es) de Uso',
						'400x400',
						1,
						array(),
						'',
						$permissao_formulario,
						true,
						false,
						3
					);
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Equipe de Assessores:</td>
			<td>
				<?php
					$sql_apoio = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
							inner join pde.usuarioresponsabilidade ur on
								ur.usucpf = u.usucpf
							inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
						where
							ur.rpustatus = 'A' and
							ur.pflcod = '" . PERFIL_EQUIPE_APOIO_GESTOR . "' and
							atiid = " . $atividade['atiid'] . "
						order by u.usucpf
					";
					$apoio = $db->carregar( $sql_apoio );
					$apoio = $apoio ? $apoio : array();
					$sql_combo2 = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf  
						where
							( u.suscod = 'A' or u.suscod = 'P' ) and
							( us.suscod = 'A' or us.suscod = 'P' ) and
							us.sisid = ". $_SESSION['sisid'] ."
						group by u.usucpf, u.usunome";
					combo_popup(
						'apoio',
						$sql_combo2,
						'Selecione o(s) Identificador(es) de Uso',
						'400x400',
						0,
						array(),
						'',
						$permissao_formulario,
						true
					);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Ano de Referência</td>
			<td>
				<?php 
					$ano = date("Y");
					$x=0;
					for($i=$ano;$i<($ano+10);$i++){
						$arrAnos[$x] = array("codigo" => $i, "descricao" => $i);
						$x++;
					}
				?>
				<?php $db->monta_combo("atiexercicio",$arrAnos,"S","Selecione...","","","","",'S') ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Meta:</td>
			<td><?= campo_textarea( 'atimetaglobal', 'N', $permissao_formulario, '', 70, 5, '' ); ?></td>
		</tr>
		<?php if( $permissao ): ?>
		<tr id="tr_botoes_acao" style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
			<td>
				<input type="button" name="botao_gravar" value="Gravar" onclick="enviar();"/>
				<input type="button" name="botao_cancelar" value="Cancelar" onclick="cancelar();"/>
			</td>
		</tr>
		<?php endif; ?>
	</table>
</form>