<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		Módulo: colunas.inc
		Finalidade: Manutenção na tabela infconcont e infconcontdet (schema financeiro)
	*/

	/*
		Exclui e depois insere dados na tabela infconcontdet	 	

	 	param integer $iccid 
		param string $concods 
		param string $ano 
	*/
	function atualiza_planocontas( $iccid, $concods, $ano )
	{
		global $db;
		$iccid = (integer) $iccid;
		if ( !$iccid )
		{
			return;
		}
		if ( $concods[0] == '' )
		{
			$concods = array();
		}

		$sql_remocao = "delete from financeiro.infconcontdet where iccid = " . $iccid;
		$db->executar( $sql_remocao );
		
		foreach ( (array) $concods as $concod )
		{
			$sql_insercao = "insert into financeiro.infconcontdet ( iccid, concod, conano ) values ( " . $iccid . ", '" . $concod . "', '" . $ano . "' )";
			$db->executar( $sql_insercao );
		}

	}
	
	/**
	 * Insere um registro na tabela elabrev.matriz.
	 *
	 * @param string $ano
	 * @param string $dsc
	 * @param string $abrev
	 * @return integer
	 */
	function insere_dados ( $ano, $dsc, $abrev, $iccobs )
	{
		global $db;
		$mtrmatriz = $mtrmatriz ? 'true' : 'false' ;

		$sql = "insert into financeiro.infconcont ( iccano, iccdsc, iccabrev, iccstatus, iccobs ) values ( '" . $ano . "', '" . $dsc . "', '" . $abrev . "', 'I', '" . $iccobs . "' )";
		
		if ( !$db->executar( $sql ) )
		{
			return 0;
		}
		return $db->pegaUm( "select currval( 'financeiro.infconcont_iccid_seq'::regclass ) as iccid" );
	}
	
	/**
	 * Insere um registro na tabela elabrev.matriz.
	 *
	 * @param integer $iccid
	 * @param string $ano
	 * @param string $dsc
	 * @param string $abrev
	 * @return integer
	 */
	function atualiza_dados ( $iccid, $ano, $dsc, $abrev, $iccobs )
	{
		global $db;
		$mtrmatriz = $mtrmatriz ? 'true' : 'false' ;

		$sql = "update financeiro.infconcont set iccano = '". $ano ."', iccabrev = '". $abrev ."', iccdsc = '". $dsc ."', iccstatus = 'A', iccobs = '". $iccobs ."' where iccid = ". $iccid;
		if ( !$db->executar( $sql ) )
		{
			return 0;
		}

	}
	
	
	/**
	 * Exclui registro na tabela financeiro.infconcontdet e financeiro.infconcont através do iccid
	 *
	 * @param integer $iccid
	 */
	function exclui_dados ( $iccid )
	{
		global $db;

		$sql_remocao = "delete from financeiro.infconcontdet where iccid = " . $iccid;
		if ( !$db->executar( $sql_remocao ) )
			{
				return 1;
			}
		$sql_remocao = "delete from financeiro.infconcont where iccid = " . $iccid;
		$db->executar( $sql_remocao );
		if ( !$db->executar( $sql_remocao ) )
			{
				return 1;
			}
		
		return 0;
			
	}	

	function pegar_planocontas( $iccid )
	{
		global $db;
		$iccid = (integer) $iccid;
		if ( !$iccid )
		{
			return array();
		}
		$sql = "select infconcontdet.concod, planoconta.condsc as descricao from financeiro.infconcontdet inner join financeiro.planoconta ON planoconta.concod = infconcontdet.concod where iccid = " . $iccid;
		
		$planoconta = $db->carregar( $sql );
		$planoconta_fixas = array();
		if ( $planoconta )
		{
			foreach ( $planoconta as $descricao )
			{
				$planoconta_fixas[] = $descricao['concod'];
			}
		}
		return $planoconta_fixas;
	}
	
	// PRÉ-PROCESSAMENTO
	
		// VARIÁVEIS	
	
			// campos da entidade matriz
			$planoconta = array();
			$iccano = $_SESSION['exercicio'];
			
			// variáveis gerais
			$modulo = $_REQUEST['modulo'];
			$mensagem_alert = '';
			$act = trim( $_REQUEST['act'] ); // indica se o formulario foi submetido
			$iccid = trim( $_REQUEST['iccid'] ); 
			$iccdsc = trim( $_REQUEST['iccdsc'] );
			$iccabrev = trim( $_REQUEST['iccabrev'] ); 
			$iccobs = trim( $_REQUEST['iccobs'] ); 

		// FIM VARIÁVEIS
		
		// MANIPULA REQUISICAO
			if ( $iccid != '' ) $where = ' where iccid <> '. $iccid;
			$sql_planocontas = "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Coluna\" onclick=\"altera_coluna('||iccid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Coluna\" onclick=\"excluir_coluna('||iccid||')\">' as acao, iccano, iccabrev, iccdsc from financeiro.infconcont ". $where ." order by iccano, iccabrev, iccdsc ";
			$cabecalho_planocontas = array( 'Ações', 'Ano', 'Abrev.', 'Título' );
			
		
			
			// verifica se formulario foi submetido para exibição
			if ( $act == 'exibir' )
			{
				// sobreescreve as variáveis declaradas como campos da entidade matriz
				$sql_matriz = "select iccano, iccdsc, iccabrev, iccobs from financeiro.infconcont where iccid = " . $iccid;
				extract( (array) $db->recuperar( $sql_matriz ) );
				$planoconta = pegar_planocontas( $iccid ) ;
				$act = 'alterar'; // ação se transforma em alteração
			}
			else if ( $act == 'alterar' || $act == 'salvar' ||  $act == 'excluir' )
			{
				if ( $act == 'salvar' ) 
				{
					$iccid = insere_dados( $iccano, $iccdsc, $iccabrev, $iccobs );
					atualiza_planocontas( $iccid, $_REQUEST['planoconta'], $iccano );
					$db->commit();
					$db->sucesso('principal/configuracao/colunas','&act=');
					exit();
				}
				
				if ( $act == 'alterar' ) 
				{
					atualiza_dados( $iccid, $iccano, $iccdsc, $iccabrev, $iccobs );
					atualiza_planocontas( $iccid, $_REQUEST['planoconta'], $iccano );
					$db->commit();
					$db->sucesso('principal/configuracao/colunas','&act=exibir&iccid='.$iccid);
					exit();
				}
				
				if ( $act == 'excluir' ) 
				{
					exclui_dados( $iccid );
					$db->commit();
					$db->sucesso('principal/configuracao/colunas','&act=');
					exit();
				}
			}
		
		// FIM MANIPULA REQUISICAO
	
	

		
	// FIM PRÉ-PROCESSAMENTO

	include APPRAIZ."includes/cabecalho.inc"; 

?>
<br/>
<script type="text/javascript">

	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

</script>
	

<? $db->cria_aba( $abacod_tela, $url, '' ); ?>
<? monta_titulo( 'Configuração do Módulo Financeiro', '' ); ?>

<form method="post" name="formulario">
	<input type="hidden" name="modulo" value="<?= $modulo ?>">
	<input type="hidden" name="iccid" value="<?= $iccid ?>">
	<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>">
	<input type="hidden" name="act" value="<?= $act ?>">
	<!-- FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
	<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<!-- DESCRICAO -->
			<td align='right' class="SubTituloDireita">Título:</td>
			<td><?= campo_textarea('iccdsc', 'S', 'S', '', 75, 3, 100 ); ?></td>
		</tr>
		<tr>
			<!-- ABREVIAÇÃO -->
			<td align='right' class="SubTituloDireita">Abreviação:</td>
			<td><?= campo_texto( 'iccabrev', 'S', 'S', '', 75, 20, '', '' ); ?></td>
		</tr>
		<tr>
			<!-- DESCRIÇÃO -->
			<td align='right' class="SubTituloDireita">Descrição:</td>
			<td><?= campo_textarea('iccobs', 'S', 'S', '', 75, 5, 500 ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'planoconta' );">
				Conta Contábil
				<input type="hidden" id="planoconta_campo_flag" name="planoconta_campo_flag" value="0"/>
			</td>
			<td>
				<div id="planoconta_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'planoconta' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="planoconta_campo_on" style="display:none;">
					<? $sql_combo = "select concod as codigo, condsc as descricao from financeiro.planoconta where conano = '" . $iccano . "' order by concod "; 
					if ( $planoconta && $planoconta[0] != '' )	
					{
						$sql_carregados = "select concod as codigo, condsc as descricao from financeiro.planoconta where conano = '" . $iccano . "' and concod in ('".implode("','",$planoconta)."') ";
						$planoconta=$db->carregar( $sql_carregados );
					}
					?>
					<? combo_popup( 'planoconta', $sql_combo, 'Selecione o(s) Planos de Conta(s)', '400x400', 0, array(), '', 'S', true, false ); ?>
				</div>
			</td>
		</tr>
		<? //if ( $planoconta )  { ?>	<script type="text/javascript"> onOffCampo( 'planoconta' ); </script> <? //} ?>

		<tr bgcolor="#cccccc">
			<td>&nbsp;</td>
			<td>
				<input type="button" name="bsubmeter" value="Salvar" class="botao" onclick="validar_cadastro();"/>&nbsp;&nbsp;
				<input type="button" name="btnovo" value="Novo" class="botao" onclick="location.href='financeiro.php?modulo=principal/configuracao/colunas&acao=R&act=';"/>
			</td>
		</tr>
	</table>
	<!-- FIM FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
</form>
<!-- LISTAGEM -->
<? $db->monta_lista( $sql_planocontas, $cabecalho_planocontas, 200, 20, '', '', '' ); ?>
<!-- FIM LISTAGEM -->
<script language="javascript">
	
	<!--
		
		
		/**
		 * Prepara formulário para alteração.
		 * 
		 * @param integer mtrid
		 * @return void
		 */
		function altera_coluna( iccid )
		{
			document.location.href = '?modulo=principal/configuracao/colunas&acao=R&act=exibir&iccid=' + iccid;
		}
		
		/**
		 * Prepara formulário para remoção.
		 * 
		 * @param integer mtrid
		 * @return void
		 */
		function excluir_coluna( iccid )
		{
			if( window.confirm( "Confirma a exclusão do item " + iccid + " em Colunas?" ) == false )
			{
				return;
			}
			document.location.href = '?modulo=principal/configuracao/colunas&acao=R&act=excluir&iccid=' + iccid;
		}
		
		/**
		 * Verifica se formulário este correto e o submete.
		 * 
		 * @return void
		 */
		function validar_cadastro()
		{

			prepara_formulario();
			var formulario = document.formulario;
			if ( !validaBranco( formulario.iccdsc, 'Título' ) ) return;
			if ( !validaBranco( formulario.iccabrev, 'Abreviação' ) ) return;
			//if ( !validaBranco( formulario.iccobs, 'Descrição' ) ) return;

			// verifica quantidade de contas, sendo necessário pelo menos uma
			if ( formulario.planoconta[0].value == '' && formulario.planoconta.options.length == 1 )
			{
				alert( 'Deve incluir pelo menos uma conta.' );
				return;
			}
			
			if ( formulario.act.value == '' ) 
				formulario.act.value = 'salvar';
			else
				formulario.act.value = 'alterar';
			
			formulario.submit();
		}
		
		<? if ( $mensagem_alert != '' ) : ?>
			alert( '<?= str_replace( "'", "\\'", $mensagem_alert ) ?>' );
		<? endif; ?>
		
	-->
	
</script>