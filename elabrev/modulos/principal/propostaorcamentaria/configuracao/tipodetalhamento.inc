<?php

	// FUNÇÕES
		
		/**
		 * Captura dados das unidades de uma proposta e seu tipo atribuído.
		 *
		 * @param unknown_type $ppoid
		 */
		function pegar_unidades_da_proposta( $ppoid )
		{
			global $db;
			$sql_unidades = <<<EOF
select
	u.unicod,
	u.unitpocod,
	u.unidsc,
	coalesce( tu.tpdcod, '0' ) as tpdcod,
	coalesce( b.total, 0 ) as limite,
	coalesce( c.despesa, 0 ) as despesa,
	coalesce( c.saldo, 0 ) as saldo
from unidade u
	inner join elabrev.limitefonte lf using ( unicod, unitpocod )
	left join elabrev.tpdunidade tu on
		tu.unitpocod = u.unitpocod and tu.unicod = u.unicod  and tu.tpustatus = 't'
	left join ( 
		select 
			u.unicod, 
			sum( coalesce( l.lmfvalor, 0 ) ) as total 
		from elabrev.limitefonte_calc2 l  
			inner join unidade u ON l.unicod = u.unicod and l.unitpocod = u.unitpocod 
			inner join elabrev.montantematriz mm using ( mtmid ) 
			inner join elabrev.propostaorcamento po on mm.ppoid=po.ppoid and po.ppoanoexercicio = '2006' and po.ppostatus='A' 
			inner join elabrev.matriz m using ( mtrid )
		where l.ppoid = $ppoid
		group by u.unicod 
		) as b ON b.unicod = u.unicod 
	left join ( 
		select 
			su.unicod, 
			sum( coalesce( vldespesa, 0 ) ) as despesa, 
			sum( coalesce( saldo, 0 ) ) as saldo 
		from elabrev.v_saldounidadefonte2 su 
		where ppoanoexercicio = '{$_SESSION["exercicio"]}'
		group by su.unicod, su.unidsc
		) as c on c.unicod = u.unicod
where
	u.unistatus = 'A' and
	u.unitpocod = 'U' 
group by
	u.unicod, u.unitpocod, u.unidsc, tu.tpdcod, b.total, c.despesa, c.saldo
order by
	u.unicod, u.unidsc
EOF;
			return $db->carregar( $sql_unidades );
		}
		
		/**
		 * Remove (ATRIBUI STATUS INATIVO) todas as entradas de
		 * tipo de detalhamento atribuído as unidades de uma
		 * determinadas proposta.
		 *
		 * @param integer $ppoid
		 * @param string $unicod
		 * @param string $unitpocod
		 * @return boolean
		 */
		function remover_tipo_unidade( $ppoid, $unicod, $unitpocod )
		{
			global $db;
			$ppoid = (integer) $ppoid;
			//$sql = "delete from elabrev.tpdunidade where ppoid = " . $ppoid;
			$sql =
				" update elabrev.tpdunidade set " .
					" tpustatus = false " .
				" where " .
					//" ppoid = " . $ppoid . " and " .
					" unicod = '". $unicod ."' and " .
					" unitpocod = '". $unitpocod . "'";
			return (boolean) $db->executar( $sql );
		}
		
		/**
		 * Remove (ATRIBUI STATUS INATIVO) todas as entradas de
		 * tipo de detalhamento atribuído as unidades de uma
		 * determinadas proposta.
		 *
		 * @param integer $ppoid
		 * @param string $unicod
		 * @param string $unitpocod
		 * @param string $tpdcod
		 * @return boolean
		 */
		function inserir_tipo_unidade( $ppoid, $unicod, $unitpocod, $tpdcod )
		{
			if ( !remover_tipo_unidade( $ppoid, $unicod, $unitpocod ) )
			{
				return false;
			}
			if ( $tpdcod == '0' )
			{
				return true;
			}
			global $db;
			$ppoid = (integer) $ppoid;
			$sql =
				" insert into elabrev.tpdunidade " .
					//" ( ppoid, tpdcod, unicod, unitpocod, tpustatus ) " .
					" ( tpdcod, unicod, unitpocod, tpustatus ) " .
				" values " .
					" ( " .
						//$ppoid . ", " .
						"'" . $tpdcod . "', " .
						"'" . $unicod. "'," .
						"'" . $unitpocod. "'," .
						"true" .
					" ) ";
			return (boolean) $db->executar( $sql );
		}
		
	// FIM FUNÇÕES

	// MANIPULA REQUISIÇÃO
		
		//$ppoid = $_REQUEST['ppoid'];
		$ppoid = '0';
		
		if ( $_REQUEST['act'] == 'gravar' )
		{
			foreach ( $_REQUEST['tipo'] as $cod => $tpdcod )
			{
				$cod = explode( '_', $cod );
				$unicod = $cod[0];
				$unitpocod = $cod[1];
				if ( !inserir_tipo_unidade( $ppoid, $unicod, $unitpocod, $tpdcod ) )
				{
					$db->insucesso( 'Ação não realizada. Não foi possível alterar os tipos de detalhamento para as unidades desejadas.' );
					exit();
				}
				$db->commit();
			}
		}
		
		// captura os tipos de detalhamento disponíveis
		$sql_tipos_detalhamento = "select tpdcod, tpddsc from elabrev.tipodetalhamento where tpdstatus = 'A' order by tpdcod ";
		$tipos_detalhamento = $db->carregar( $sql_tipos_detalhamento );
		
		// captura o nome da proposta
		//$sql_proposta = "select ppodsc from elabrev.propostaorcamento where ppoid = " . $ppoid;
		//$ppodsc = $db->pegaUm( $sql_proposta );
	
	// FIM MANIPULA REQUISIÇÃO
	
?>
<? include APPRAIZ."includes/cabecalho.inc"; ?>
<br />
<? monta_titulo( $titulo_modulo, 'Lista de Unidades Orçamentárias que possuem limites' ); ?>
<form action="" method="post" name="formulario">
	<input type="hidden" name="act" value="gravar" />
	<!-- CABECALHO -->
	<table class="tabela" cellspacing="0" width="95%" bgcolor="#f5f5f5" cellpadding="2" border="0" align="center">
		<tr>
			<td>
				<table cellSpacing="0" width="100%" cellPadding="2" border="0" align="center">
					<tr>
						<td width="20" align="left" class="TituloTela" style="color:#000000;">
							Órgão:
						</td>
						<td width="100%" align="left" class="TituloTela" style="color:#000000;">
							&nbsp;&nbsp;<?php echo CODIGO_ORGAO_SISTEMA. ' '. NOME_ORGAO_SISTEMA; ?>
						</td>
					</tr>
					<!--
					<tr>
						<td align="left" class="TituloTela" style="color:#000000;">
							Proposta:
						</td>
						<td align="left" class="TituloTela" style="color:#000000;">
							&nbsp;&nbsp;<?= $ppodsc ?>
						</td>
					</tr>
					-->
				</table>
			</td>
		</tr>
	</table>
	<!-- FIM CABECALHO -->
	<script type="text/javascript">
	
		/**
		 * Volta para a tela de adminstração de propostas.
		 * 
		 * @return void
		 */
		function voltar()
		{
			window.location.href = 'elabrev.php?modulo=principal/propostaorcamentaria/conMontante&acao=A';
		}
	
	</script>
	<!-- UNIDADES -->
	<?php $unidades = pegar_unidades_da_proposta( $ppoid ); ?>
	<? if ( !$unidades ) : ?>
		<p style="text-align: center; color: #aa3030;">
			Não há Unidade Orçamentária que possua limite atribuído
			<!--
			<br/><br/>
			<input type="button" value="Voltar" onclick="javascript:voltar();" />
			-->
		</p>
	<? else : ?>
		<script type="text/javascript">
		
			/**
			 * Marca um td de uma unidade para um determinado tipo, e desmarca os outros.
			 * 
			 * @param string opcao qual das opções foi selecionada
			 * @param string linha qual linha foi acionada
			 * @return void
			 */
			function marcar_td_tipo( opcao, linha )
			{
				<? if ( $tipos_detalhamento ) : ?>
					<? foreach ( $tipos_detalhamento as $tipo ) : ?>
						document.getElementById( 'td_<?= $tipo['tpdcod'] ?>_' + linha ).style.backgroundColor = '';
					<? endforeach; ?>
				<? endif; ?>
				//document.getElementById( 'td_0_' + linha ).style.backgroundColor = '';
				document.getElementById( 'td_' + opcao + '_' + linha ).style.backgroundColor = '#f0f090';
			}
			
			/**
			 * Altera a opção de todas as unidades para uma opção.
			 * 
			 * @param string
			 * @return void
			 */
			function marcar_todos( opcao )
			{
				var check, td, i;
				<? if ( $unidades && $tipos_detalhamento ) : ?>
					<? foreach ( $tipos_detalhamento as $tipo ) : ?>
						for ( var i = <?= count( $unidades ) ?> - 1; i >= 0; i-- )
						{
							check = document.getElementById( 'momento_check_<?= $tipo['tpdcod'] ?>_' + i );
							td = document.getElementById( 'td_<?= $tipo['tpdcod'] ?>_' + i );
							if ( opcao == '<?= $tipo['tpdcod'] ?>' )
							{
								check.checked = true;
								td.style.backgroundColor = '#f0f090';
							}
							else
							{
								td.style.backgroundColor = '';
							}
						}
					<? endforeach; ?>
				<? endif; ?>
			}
			
			/**
			 * Sumete o formulário da página.
			 * 
			 * @return void
			 */
			function submeter_formulario()
			{
				document.formulario.submit();
			}
		
		</script>
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
						<strong>Unidade</strong>
					</td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
						<strong>Limite</strong>
					</td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
						<strong>Despesa</strong>
					</td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
						<strong>Saldo</strong>
					</td>
					<? foreach ( $tipos_detalhamento as $tipo ) : ?>
						<td width="50" align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<strong>
								<a href="javascript:marcar_todos( '<?= $tipo['tpdcod'] ?>' );">
									<?= $tipo['tpddsc'] ?>
								</a>
							</strong>
						</td>
					<? endforeach; ?>
					<!--
					<td width="50" align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
						<strong>
							<a href="javascript:marcar_todos( '0' );">
								n/a
							</a>
						</strong>
					</td>
					-->
				</tr>
			</thead>
			<? foreach ( $unidades as $key => $unidade ) : ?>
				<? $cod = $unidade['unicod'] . '_' . $unidade['unitpocod']; ?>
				<? $cor = $key % 2 ? '#F7F7F7' : '' ; ?>
				<tr bgcolor="<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?= $cor ?>';">
					<td><?= $unidade['unicod'] ?> - <?= $unidade['unidsc'] ?></td>
					<td align="right"><?= number_format( $unidade['limite'], 2, ',', '.' ) ?></td>
					<td align="right"><?= number_format( $unidade['despesa'], 2, ',', '.' ) ?></td>
					<td align="right">
						<font color="#<?= $unidade['saldo'] < 0 ? 'dd0000' : '009000' ?>">
							<?= number_format( $unidade['saldo'], 2, ',', '.' ) ?>
						</font>
					</td>
					<? $tipos = array(); ?>
					<? if ( $tipos_detalhamento ) : ?>
						<? foreach ( $tipos_detalhamento as $tipo ) : ?>
							<? array_push( $tipos, $tipo['tpdcod'] ); ?>
							<td align="center" id="td_<?= $tipo['tpdcod'] ?>_<?= $key ?>">
								<input id="momento_check_<?= $tipo['tpdcod'] ?>_<?= $key ?>" onclick="javascript:marcar_td_tipo( '<?= $tipo['tpdcod'] ?>', '<?= $key ?>' );" type="radio" name="tipo[<?= $cod ?>]" value="<?= $tipo['tpdcod'] ?>" <?= $unidade['tpdcod'] == $tipo['tpdcod'] ? 'checked="checked"' : '' ?> />
							</td>
						<? endforeach; ?>
					<? endif; ?>
					<!--
					<td align="center" id="td_0_<?= $key ?>">
						<? $tipo = in_array( $unidade['tpdcod'], $tipos ) ? $unidade['tpdcod'] : '0' ; ?>
						<input id="momento_check_0_<?= $key ?>" onclick="javascript:marcar_td_tipo( '0', '<?= $key ?>' );" type="radio" name="tipo[<?= $cod ?>]" value="0" <?= $tipo == '0' ? 'checked="checked"' : '' ?> />
						
					</td>
					-->
					<? if ( $tipo ) : ?>
					<script type="text/javascript">
						javascript:marcar_td_tipo( '<?= $tipo ?>', '<?= $key ?>' );
					</script>
					<? endif; ?>
				</tr>
			<? endforeach; ?>
		</table>
		<p style="text-align: center;">
			<input type="button" value="Salvar" onclick="javascript:submeter_formulario();" />
			<!--
			<input type="button" value="Voltar" onclick="javascript:voltar();" />
			-->
		</p>
	<? endif; ?>
	<!-- FIM UNIDADES -->
</form>