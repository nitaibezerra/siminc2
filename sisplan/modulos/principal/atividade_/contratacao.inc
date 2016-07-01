<?php
$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid'],
	'idaditivo' => $_REQUEST['idaditivo']
);

$evento = $_REQUEST['evento'] ? $_REQUEST['evento'] : 'incluir';
$sadid = $_REQUEST['sadid'] ? $_REQUEST['sadid'] : '';

function mascaraglobal($value, $mask) {
		$casasdec = explode(",", $mask);
		// Se possui casas decimais
		if($casasdec[1])
			$value = sprintf("%01.".strlen($casasdec[1])."f", $value);
	
		$value = str_replace(array("."),array(""),$value);
		if(strlen($mask)>0) {
			$masklen = -1;
			$valuelen = -1;
			while($masklen>=-strlen($mask)) {
				if(substr($mask,$masklen,1) == "#") {
					$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
					$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
				$masklen--;
			}
		}
		return $valueformatado;
	}

switch( $_REQUEST['evento'] ){

	case 'incluir':

		if ( !$_REQUEST['cttid'] )	{
			$sql = sprintf(
				"insert into sisplan.contratacao ( 
				  	mdcid,
					cttnumprocesso,
					cttnumnotacredito,
					usucpfcadastrador,
					atiid
				  ) values ( 
				  %d, 	-- mdcid 
				  '%s', -- cttnumprocesso
				  '%s', -- cttnumnotacredito
				  '%s', -- usucpfcadastrador
				  %d	-- atiid
				  ) returning cttid ",
				$_REQUEST['mdcid'],
				$_REQUEST['cttnumprocesso'],
				$_REQUEST['cttnumnotacredito'],
				$_SESSION['usucpf'],
				$_REQUEST['atiid']
				);

			try 
				{
					$cttid = (integer) $db->pegaUm( $sql );

					if ( $cttid ) 
					{


						$sql = sprintf(
							"insert into sisplan.situacaotecnica ( 
							  	sttdtprevista,
								sttdtconclusao,
								cttid,
								usucpfcadastrador,
								sttidsituacao
							  ) values ( 
							  %s, 	-- sttdtprevista 
							  %s, 	-- sttdtconclusao
							  %d, 	-- cttid
							  '%s', -- usucpfcadastrador
							  %d	-- sttidsituacao
							  )",
							$_REQUEST['sttdtprevista'] ? "'" . $_REQUEST['sttdtprevista'] . "'" : 'NULL',
							$_REQUEST['sttdtconclusao'] ? "'" . $_REQUEST['sttdtconclusao'] . "'" : 'NULL',
							$cttid,
							$_SESSION['usucpf'],
							$_REQUEST['sttidsituacao']
							);

						$db->executar( $sql );

						$db->commit();
						$db->sucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
					}
				}
			catch ( Exception $e )
				{
					dbg($e,1);
					$db->rollback();
					die('<script>alert(\'Não foi possível realizar a operação!\'); history.go(-1);</script>');
				}

		} else {

			$sql = sprintf(
							"update sisplan.contratacao set 
								mdcid = %d,
								cttnumprocesso = '%s',
								cttnumnotacredito = '%s'
						where 
					atiid = %d",
				$_REQUEST['mdcid'],
				$_REQUEST['cttnumprocesso'],
				$_REQUEST['cttnumnotacredito'],
				$_REQUEST['atiid'] );

				try 
				{
					if ( $db->executar( $sql ) ) 
					{

						$sql = sprintf("update sisplan.situacaotecnica set sttstatus = 'I'
								where 
							cttid = %d",
						$_REQUEST['cttid'] );
						
						$db->executar( $sql );

						$sql = sprintf(
							"insert into sisplan.situacaotecnica ( 
							  	sttdtprevista,
								sttdtconclusao,
								cttid,
								usucpfcadastrador,
								sttidsituacao
							  ) values ( 
							  %s, 	-- sttdtprevista 
							  %s, 	-- sttdtconclusao
							  %d, 	-- cttid
							  '%s', -- usucpfcadastrador
							  %d	-- sttidsituacao
							  )",
							$_REQUEST['sttdtprevista'] ? "'" . $_REQUEST['sttdtprevista'] . "'" : 'NULL',
							$_REQUEST['sttdtconclusao'] ? "'" . $_REQUEST['sttdtconclusao'] . "'" : 'NULL',
							$_REQUEST['cttid'],
							$_SESSION['usucpf'],
							$_REQUEST['sttidsituacao']
							);

						$db->executar( $sql );

						if ( $_REQUEST['staidsituacao'] ) {
							$sql = sprintf("update sisplan.situacaoadmin set stastatus = 'I'
									where 
								cttid = %d",
							$_REQUEST['cttid'] );
						
							$db->executar( $sql );

							$sql = sprintf(
								"insert into sisplan.situacaoadmin ( 
								  	stadtprevista,
									stadtconclusao,
									cttid,
									usucpfcadastrador,
									staidsituacao
								  ) values ( 
								  %s, 	-- stadtprevista 
								  %s, 	-- stadtconclusao
								  %d, 	-- cttid
								  '%s', -- usucpfcadastrador
								  %d	-- staidsituacao
								  )",
								$_REQUEST['stadtprevista'] ? "'" . $_REQUEST['stadtprevista'] . "'" : 'NULL',
								$_REQUEST['stadtconclusao'] ? "'" . $_REQUEST['stadtconclusao'] . "'" : 'NULL',
								$_REQUEST['cttid'],
								$_SESSION['usucpf'],
								$_REQUEST['staidsituacao']
								);

							$db->executar( $sql );
						}

						if ( $_REQUEST['stjidsituacao'] ) {
							$sql = sprintf("update sisplan.situacaojuridica set stjstatus = 'I'
									where 
								cttid = %d",
							$_REQUEST['cttid'] );
						
							$db->executar( $sql );

							$sql = sprintf(
								"insert into sisplan.situacaojuridica ( 
								  	stjdtprevista,
									stjdtconclusao,
									cttid,
									usucpfcadastrador,
									stjidsituacao
								  ) values ( 
								  %s, 	-- stjdtprevista 
								  %s, 	-- stjdtconclusao
								  %d, 	-- cttid
								  '%s', -- usucpfcadastrador
								  %d	-- stjidsituacao
								  )",
								$_REQUEST['stjdtprevista'] ? "'" . $_REQUEST['stjdtprevista'] . "'" : 'NULL',
								$_REQUEST['stjdtconclusao'] ? "'" . $_REQUEST['stjdtconclusao'] . "'" : 'NULL',
								$_REQUEST['cttid'],
								$_SESSION['usucpf'],
								$_REQUEST['stjidsituacao']
								);

							$db->executar( $sql ); 
						}

						if ( $_REQUEST['stcidsituacao'] ) {
							$sql = sprintf("update sisplan.situacaocontratacao set stcstatus = 'I'
									where 
								cttid = %d",
							$_REQUEST['cttid'] );
						
							$db->executar( $sql );

							$sql = sprintf(
								"insert into sisplan.situacaocontratacao ( 
								  	stcdtinicio,
									stcdtprevisao,
									stcdtconclusao,
									stcnotaempenho,
									stcnumerocontrato,
									stccontratado,
									stcvalorcontratado,
									tpcid,
									cttid,
									usucpfcadastrador,
									stcidsituacao,
								  	stcdtpublicacaoresult,
								  	stcdtpublicacaoextrato,
								  	stcidlocalpubresult,
								  	stcidlocalpubextrato
								  ) values ( 
								  %s, 	-- stcdtinicio 
								  %s, 	-- stcdtprevisao
								  %s, 	-- stcdtconclusao
								  '%s',	-- stcnotaempenho
								  '%s',	-- stcnumerocontrato
								  '%s',	-- stccontratado
								  '%s',	-- stcvalorcontratado
								  %s,	-- tpcid
								  %d, 	-- cttid
								  '%s', -- usucpfcadastrador
								  %d,	-- stcidsituacao
								  %s,	-- stcdtpublicacaoresult
								  %s,	-- stcdtpublicacaoextrato
								  %s,	-- stcidlocalpubresult
								  %s	-- stcidlocalpubextrato
								  )",
								$_REQUEST['stcdtinicio'] ? "'" . $_REQUEST['stcdtinicio'] . "'" : 'NULL',
								$_REQUEST['stcdtprevisao'] ? "'" . $_REQUEST['stcdtprevisao'] . "'" : 'NULL',
								$_REQUEST['stcdtconclusao'] ? "'" . $_REQUEST['stcdtconclusao'] . "'" : 'NULL',
								$_REQUEST['stcnotaempenho'],
								$_REQUEST['stcnumerocontrato'],
								str_replace( '.', '', str_replace('-','',$_REQUEST['stccontratado']) ),
								$_REQUEST['stcvalorcontratado'] ? str_replace( ',', '.', str_replace('.','',$_REQUEST['stcvalorcontratado']) ) : 0,
								$_REQUEST['tpcid'] != '' ? "".$_REQUEST['tpcid']."" : 'NULL',
								$_REQUEST['cttid'],
								$_SESSION['usucpf'],
								$_REQUEST['stcidsituacao'],
								$_REQUEST['stcdtpublicacaoresult'] ? "'" . $_REQUEST['stcdtpublicacaoresult'] . "'" : 'NULL',
								$_REQUEST['stcdtpublicacaoextrato'] ? "'" . $_REQUEST['stcdtpublicacaoextrato'] . "'" : 'NULL',
								$_REQUEST['stcidlocalpubresult'] != '' ? "".$_REQUEST['stcidlocalpubresult']."" : 'NULL',
								$_REQUEST['stcidlocalpubextrato'] != '' ? "".$_REQUEST['stcidlocalpubextrato']."" : 'NULL'
								);

							$db->executar( $sql ); 
						}

						if ( $_REQUEST['steidsituacao'] ) {
							$sql = sprintf("update sisplan.situacaoexecucao set stestatus = 'I'
									where 
								cttid = %d",
							$_REQUEST['cttid'] );
						
							$db->executar( $sql );

							$sql = sprintf(
								"insert into sisplan.situacaoexecucao ( 
								  	stedtinicio,
									stedttermino,
									stenumparcelas,
									spcid,
									stedtprestacao,
									cttid,
									steidsituacao,
									usucpfcadastrador

								  ) values ( 
								  %s, 	-- stedtinicio 
								  %s, 	-- stedttermino
								  %d, 	-- stenumparcelas
								  %s,	-- spcid
								  %s,	-- stedtprestacao
								  %d, 	-- cttid
								  %s,	-- steidsituacao
								  '%s' -- usucpfcadastrador
								  )",
								$_REQUEST['stedtinicio'] ? "'" . $_REQUEST['stedtinicio'] . "'" : 'NULL',
								$_REQUEST['stedttermino'] ? "'" . $_REQUEST['stedttermino'] . "'" : 'NULL',
								$_REQUEST['stenumparcelas'],
								$_REQUEST['spcid'] != '' ? "".$_REQUEST['spcid']."" : 'NULL',
								$_REQUEST['stedtprestacao'] ? "'" . $_REQUEST['stedtprestacao'] . "'" : 'NULL',
								$_REQUEST['cttid'],
								$_REQUEST['steidsituacao'] ? "'" . $_REQUEST['steidsituacao'] . "'" : NULL,
								$_SESSION['usucpf']
								);

							$db->executar( $sql ); 
						}


						if ( $_REQUEST['saddtaditivo'] ) {
							
							$sql = sprintf(
									"insert into sisplan.situacaoaditivo ( 
									  	saddtaditivo,
										sadvlraditivo,
										sadjustificativa,
										saddttermino,
										tpaid,
										cttid,
										usucpfcadastrador

									  ) values ( 
									  %s, 	-- saddtaditivo 
									  %s, 	-- sadvlraditivo
									  '%s',	-- sadjustificativa
									  %s, 	-- saddttermino
									  %d, 	-- tpaid
									  %d, 	-- cttid
									  '%s' -- usucpfcadastrador
									  )",
									$_REQUEST['saddtaditivo'] ? "'" . $_REQUEST['saddtaditivo'] . "'" : 'NULL',
									$_REQUEST['sadvlraditivo']? str_replace( ',', '.', str_replace('.','',$_REQUEST['sadvlraditivo']) ):'NULL',
									$_REQUEST['sadjustificativa'],
									$_REQUEST['saddttermino'] ? "'" . $_REQUEST['saddttermino'] . "'" : 'NULL',
									$_REQUEST['tpaid'],
									$_REQUEST['cttid'],
									$_SESSION['usucpf']
									);
								
							$db->executar( $sql ); 
						}

						$db->commit();
						$db->sucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
					} else {
					$db->rollback();
					$db->insucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
						}
				}
			catch ( Exception $e )
				{
					dbg($e,1);
					$db->rollback();
					die('<script>alert(\'Não foi possível realizar a operação!\'); history.go(-1);</script>');
				}

		}
		//redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );					
		break;

	case 'aditivo':


					$sql = sprintf("update sisplan.situacaoaditivo set 
								saddtaditivo = %s,
								sadvlraditivo = %s,
								sadjustificativa = '%s',
								saddttermino = %s,
								tpaid = %d,
								cttid = %d,
								usucpfcadastrador = '%s'
								where 
							sadid = %d",
							$_REQUEST['saddtaditivo'] ? "'" . $_REQUEST['saddtaditivo'] . "'" : 'NULL',
							str_replace( ',', '.', str_replace('.','',$_REQUEST['sadvlraditivo']) ),
							$_REQUEST['sadjustificativa'],
							$_REQUEST['saddttermino'] ? "'" . $_REQUEST['saddttermino'] . "'" : 'NULL',
							$_REQUEST['tpaid'],
							$_REQUEST['cttid'],
							$_SESSION['usucpf'],
							$_REQUEST['sadid'] );

				try 
				{
					if ( $db->executar( $sql ) ) 
					{
						$db->commit();
						$db->sucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
					} else {
					$db->rollback();
					$db->insucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
						}
				}
			catch ( Exception $e )
				{
					dbg($e,1);
					$db->rollback();
					die('<script>alert(\'Não foi possível realizar a operação!\'); history.go(-1);</script>');
				}

	default:

		break;

}

	if ( $atividade )
		{
			$contratacao = atividade_pegar_contratacao( $_REQUEST['atiid'] );
//dbg($contratacao);
			if ( $contratacao ) extract( $contratacao );
			$aditivo = atividade_pegar_aditivo( $_REQUEST['sadid'] );
			if ( $aditivo ) extract( $aditivo );
		}

// ----- VERIFICA PERMISSÃO
$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário
$permissao_atribuir_gerente = $permissao_formulario;
$permissao_atribuir_apoio   = $permissao_formulario;

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

/*
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba(
	$abacod_tela,
	$url,
	'&atiid=' . $atividade['atiid']
);
monta_titulo(
	$titulo_modulo,
	$permissao ? '&nbsp;' : '<img src="../imagens/preview.gif" align="absmiddle"/> Atividade disponível apenas para visualização'
);
*/

/**
 * Captura todos os dados dos usuários com um perfil de uma atividade.
 * 
 * @return string[]
 */
function atividade_pegar_responsaveis( $atividade, $perfil ){
	global $db;
	$sql = <<<EOT
		select
			u.*
		from pde.usuarioresponsabilidade ur
			inner join seguranca.usuario u on
				u.usucpf = ur.usucpf
		where
			ur.rpustatus = 'A' and
			u.usustatus = 'A' and
			ur.atiid = %d and
			ur.pflcod = %d
EOT;
	$sql = sprintf( $sql, $atividade, $perfil );
	$usuarios = $db->carregar( $sql );
	return $usuarios ? $usuarios : array();
}



// captura dados de responsáveis por atividades superiores
$responsaveis_gerente_superiores = array();
$responsaveis_apoio_superiores = array();
$rastro = atividade_pegar_rastro( $atividade['numero'] );
array_pop( $rastro );
foreach ( $rastro as $item_rastro )
{
	$responsaveis_gerente = atividade_pegar_responsaveis( $item_rastro['atiid'], PERFIL_GERENTE );
	$responsaveis_apoio = atividade_pegar_responsaveis( $item_rastro['atiid'], PERFIL_EQUIPE_APOIO_GERENTE );
	foreach ( $responsaveis_gerente as $responsavel_gerente )
	{
		$novo_item = array(
			'numero_atividade' => $item_rastro['numero'],
			'nome_atividade'   => $item_rastro['atidescricao'],
			'nome_usuario'     => $responsavel_gerente['usunome'],
			'cpf_usuario'      => $responsavel_gerente['usucpf'],
			'atiid'            => $item_rastro['atiid']
		);
		array_push( $responsaveis_gerente_superiores, $novo_item );
	}
	foreach ( $responsaveis_apoio as $responsavel_apoio )
	{
		$novo_item = array(
			'numero_atividade' => $item_rastro['numero'],
			'nome_atividade'   => $item_rastro['atidescricao'],
			'nome_usuario'     => $responsavel_apoio['usunome'],
			'cpf_usuario'      => $responsavel_apoio['usucpf'],
			'atiid'            => $item_rastro['atiid']
		);
		array_push( $responsaveis_apoio_superiores, $novo_item );
	}
}

extract( $atividade ); # mantém o formulário preenchido

?>
<script language="JavaScript" src="../includes/funcoes.js"></script> 
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script type="text/javascript" src="../includes/prototype-1.6.0.3.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript">
	var nao_se_aplica = <?=NAO_SE_APLICA ?>;
	var concluida = <?=CONCLUIDA ?>;

	function enviar(){

		var d = document;
		var f = d.formulario;
		var msg = '';

		if (f.mdcid.value == ''){
			msg += 'Modalidade de Contratação\n';
		} 

		if (f.cttnumprocesso.value == ''){
			msg += 'Número do Processo IPHAN\n';
		} 

		if (f.sttidsituacao && f.sttidsituacao.value != '' ) {
			if (f.sttidsituacao.value != nao_se_aplica) {
				if (f.sttidsituacao.value == ''){
					msg += 'Situação da Instrução Técnica\n';
				} 

				if (f.sttdtprevista.value == ''){
					msg += 'Data de Conclusão prevista de Situação da Instrução Técnica\n';
				} 

				if (f.sttdtconclusao.value == ''){
					msg += 'Data de Conclusão da Situação da Instrução Técnica\n';
				} 
			} else {
				if (f.sttidsituacao.value == ''){
					msg += 'Situação da Instrução Técnica\n';
				} 
			}
		} else {
				if (f.sttidsituacao.value == ''){
					msg += 'Situação da Instrução Técnica\n';
				} 
		}
		

		if (f.staidsituacao && f.staidsituacao.value != '' ){
			if (f.staidsituacao.value != nao_se_aplica) {
				if (f.staidsituacao.value == ''){
					msg += 'Situação da Instrução Administrativa\n';
				}

				if (f.stadtprevista.value == ''){
					msg += 'Data de Conclusão prevista da Situação da Instrução Administrativa\n';
				}

				if (f.stadtconclusao.value == ''){
					msg += 'Data de Conclusão da Situação da Instrução Administrativa\n';
				}
			} else {
				if (f.staidsituacao.value == ''){
					msg += 'Situação da Instrução Administrativa\n';
				}
			}
		} else {
				if (f.staidsituacao && f.staidsituacao.value == ''){
					msg += 'Situação da Instrução Administrativa\n';
				}
		}

		if (f.stjidsituacao && f.stjidsituacao.value != '' ){
			if (f.stjidsituacao.value != nao_se_aplica) {
				if (f.stjidsituacao.value == ''){
					msg += 'Situação da Análise Jurídica\n';
				}

				if (f.stjdtprevista.value == ''){
					msg += 'Data de Conclusão prevista da Situação da Análise Jurídica\n';
				}

				if (f.stjdtconclusao.value == ''){
					msg += 'Data de Conclusão da Situação da Análise Jurídica';
				}
			} else {
				if (f.stjidsituacao.value == ''){
					msg += 'Situação da Análise Jurídica\n';
				}
			}
		} else {
				if (f.stjidsituacao && f.stjidsituacao.value == ''){
					msg += 'Situação da Análise Jurídica\n';
				}
		}


		if (f.stcidsituacao && f.stcidsituacao.value != '' ){
			if (f.stcidsituacao.value != nao_se_aplica) {
				if (f.stcidsituacao.value == ''){
					msg += 'Situação da Contratação\n';
				}

				if (f.stcdtinicio.value == ''){
					msg += 'Data de Início da Contratação\n';
				}

				if (f.stcdtprevisao.value == ''){
					msg += 'Data de Conclusão Prevista da Situação da Contratação\n';
				}

				if (f.stcdtconclusao.value == ''){
					msg += 'Data de Conclusão da Situação da Contratação\n';
				}

				if (f.stcnotaempenho.value == ''){
					msg += 'Nota de Empenho\n';
				}

				if (f.stcnumerocontrato.value == ''){
					msg += 'Número do Contrato\n';
				}
				
				if (f.stcdtpublicacaoresult.value == ''){
					msg += 'Data de Publicação do resultado\n';
				}

				if (f.stcidlocalpubresult.value == ''){
					msg += 'Local de Publicação\n';
				}

				if (f.stcdtpublicacaoextrato.value == ''){
					msg += 'Data de Publicação do extrato\n';
				}

				if (f.stcidlocalpubextrato.value == ''){
					msg += 'Local de Publicação\n';
				}

				if (f.stccontratado.value == ''){
					msg += 'Contratado\n';
				}

				if (f.tpcid.value == ''){
					msg += 'Tipo de Contratado\n';
				}
				if (f.stcvalorcontratado.value == ''){
					msg += 'Valor contratado\n';
				}
			} else {
				if (f.stcidsituacao.value == ''){
					msg += 'Situação da Contratação\n';
				}
			}
		} else {
				if (f.stcidsituacao && f.stcidsituacao.value == ''){
					msg += 'Situação da Contratação\n';
				}
		}


		if (f.steidsituacao && f.steidsituacao.value != '' ){
			if (f.steidsituacao.value != nao_se_aplica) {
				if (f.steidsituacao.value == ''){
					msg += 'Situação da Execução\n';
				}

				if (f.stedtinicio.value == ''){
					msg += 'Data de Início\n';
				}

				if (f.stedttermino.value == ''){
					msg += 'Data de Término\n';
				}

				if (f.stenumparcelas.value == ''){
					msg += 'Número de Parcelas\n';
				}

				if (f.spcid.value == ''){
					msg += 'Prestação de Contas\n';
				}

				if (f.stedtprestacao.value == ''){
					msg += 'Data de Prestação de Contas\n';
				}
			} else {
				if (f.steidsituacao.value == ''){
					msg += 'Situação da Execução\n';
				}
			}
		} else {
				if (f.steidsituacao && f.steidsituacao.value == ''){
					msg += 'Situação da Execução\n';
				}
		}

		if ( f.tpaid && f.tpaid.value != '' ){
			if (f.saddtaditivo.value == ''){
				msg += 'Data do aditivo\n';
			}
			
			if (f.sadporcentocontrato.value == ''){
				msg += '% do total do contrato\n';
			}

			if (f.saddttermino.value == ''){
				msg += 'Nova data de término do contrato\n';
			}

			if (f.sadjustificativa.value == ''){
				msg += 'Justificativa\n';
			}
		}else{
			if (f.tpaid && f.tpaid.value == ''){
				msg+= 'Aditivo\n';
			}
		}


		if (msg != ''){
			alert('Os campos listados são obrigatórios e devem ser preenchidos:\n'+msg);
			return false;
		}

		document.formulario.sadid.value = '';
		document.formulario.submit();
	}

		/**
		 * Transforma uma string com vírgulas e pontos em um número com decimal.
		 * 
		 * @param string
		 * @return float
		 */
		function desformata_numero( valor )
		{
			if ( valor == null )
			{
				return 0.0;
			}

			valor = valor.replace( /\./g, '' );
			valor = valor.replace( ',', '.' );
			if ( valor[valor.length] == '.' )
			{
				valor += '0';
			}
			if ( valor.length == 0 )
			{
				return 0.0;
			}

			return valor;
		}

	function calcula_percentual_aditivo(vlr) {

		var d = document.formulario;

		var valoraditivo = desformata_numero(vlr);
//alert(valoraditivo);
		var valorcontrato = desformata_numero(d.stcvalorcontratado.value);
//alert(parseFloat((valoraditivo/valorcontrato)*100));
	
		d.sadporcentocontrato.value = (((valoraditivo/valorcontrato)*10000));

	}

	function formata_campo_contratado(){
		var d = document.formulario.getElementById('stccontratado');
		d.onkeypress = 'mascaraglobal("###&###-#-##")';
	}


	function enviarAditivo(){
		document.formulario.evento.value = 'aditivo';
		document.formulario.submit();
	}

	function EditarAditivo( sadid ){
		document.formulario.sadid.value = sadid;
		document.formulario.evento.value = '';
		document.formulario.submit();
	}

	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}

	function verifica_tipo_aditivo( tipo )
	{
		
		var campotipo = document.formulario.tpaid.value;

		if ( campotipo == 1 ) {
			document.formulario.sadvlraditivo.value = '';
			document.formulario.sadvlraditivo.disabled = true;
			document.formulario.saddttermino.disabled = false;
			return true;
		} else if ( campotipo == 2 ) {
			document.formulario.saddttermino.value = '';
			document.formulario.saddttermino.disabled = true;
			document.formulario.sadvlraditivo.disabled = false;
			return true;
		} else {
			document.formulario.saddttermino.disabled = false;
			document.formulario.sadvlraditivo.disabled = false;
			return true;
		}  
	}
	
</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<form method="post" name="formulario">
				<input type="hidden" name="evento" value="<?=$evento?>"/>
				<input type="hidden" name="cttid" value="<?=$cttid?>"/>
				<input type="hidden" name="atiid" value="<?=$atiid?>"/>
				<input type="hidden" name="sadid" value="<?=$sadid?>"/>
				<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
					<tr style="background-color: #cccccc">
						<td align='left' colspan="2"><b>Responsável</b></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							Modalidade de contratação:
						</td>
						<td><?php
					$sql = sprintf( "SELECT mdcid as codigo, mdcdsc AS descricao FROM sisplan.modalidadecontratacao WHERE mdcstatus = 'A' ORDER BY 2" );
						$db->monta_combo("mdcid",$sql,$permissao_formulario,'&nbsp','','','','150','S','mdcid');
						 ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							Número do processo IPHAN:
						</td>
						<td>
							<?=
							campo_texto('cttnumprocesso','S',$permissao_formulario,'',40,20,'','','','','','id="atinumprocesso"');
							?>
						</td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							Número da Nota de Crédito:
						</td>
						<td>
							<?=
							campo_texto('cttnumnotacredito','N',$permissao_formulario,'',40,20,'','','','','','id="atinotacredito"');
							?>
						</td>
					</tr>

					<tr>
						<td align='left' colspan="2"><hr></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; bold;">
							<b>1. Situação da instrução técnica:</b>
						</td>
						<td><?php
					$sql = sprintf( "SELECT stuid as codigo, studsc AS descricao FROM sisplan.situacao WHERE stustatus = 'A' ORDER BY 2" );
						$db->monta_combo("sttidsituacao",$sql,$permissao_formulario,'&nbsp','','','','150','S','sttidsituacao');
						 ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							1.1 Data de conclusão prevista:
						</td>
						<td><?= campo_data( 'sttdtprevista', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão prevista.' ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							1.2 Data de conclusão:
						</td>
						<td><?= campo_data( 'sttdtconclusao', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão.' ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							1.3 Data de conclusão:
						</td>
						<td><input type="button" name="botao" value="Anexar arquivo" onclick="javascript:window.open('sisplan.php?modulo=principal/atividade_/anexo&acao=A&atiid='+<?=$atiid ?>+'&tptid=1', 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');"/></td>
					</tr>
					<tr>
						<td align='left' colspan="2"><hr></td>
					</tr>
			<?php 
				if ( (verificasituacaocontratacao( $cttid, 'situacaotecnica', NAO_SE_APLICA )) || (verificasituacaocontratacao( $cttid, 'situacaotecnica', CONCLUIDA )) ) { ?>
					<tr>
					
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; bold;">
							<b>2. Situação da instrução administrativa:</b>
						</td>

						<td><?php
						$sql = sprintf( "SELECT stuid as codigo, studsc AS descricao FROM sisplan.situacao WHERE stustatus = 'A' ORDER BY 2" );
						$db->monta_combo("staidsituacao",$sql,$permissao_formulario,'&nbsp','','','','150','S','staidsituacao');
						 ?></td>
					</tr>

					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							2.1 Data de conclusão prevista:
						</td>
						<td><?= campo_data( 'stadtprevista', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão prevista.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							2.2 Data de conclusão:
						</td>
						<td><?= campo_data( 'stadtconclusao', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão.' ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							2.3 anexar documento:
						</td>
						<td><input type="button" name="botao" value="Anexar arquivo" onclick="javascript:window.open('sisplan.php?modulo=principal/atividade_/anexo&acao=A&atiid='+<?=$atiid ?>+'&tptid=2', 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');"/></td>
					</tr>
					<tr>
						<td align='left' colspan="2"><hr></td>
					</tr>
			<?php // } ?>

			<?php 
				if ( (verificasituacaocontratacao( $cttid, 'situacaoadmin', NAO_SE_APLICA)) || (verificasituacaocontratacao( $cttid, 'situacaoadmin', CONCLUIDA) )) { ?>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; bold;">
							<b>3. Situação da análise jurídica:</b>
						</td>
						<td><?php
					$sql = sprintf( "SELECT stuid as codigo, studsc AS descricao FROM sisplan.situacao WHERE stustatus = 'A' ORDER BY 2" );
						$db->monta_combo("stjidsituacao",$sql,$permissao_formulario,'&nbsp','','','','150','S','stjidsituacao');
						 ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							3.1 Data de conclusão prevista:
						</td>
						<td><?= campo_data( 'stjdtprevista', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão prevista.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							3.2 Data de conclusão:
						</td>
						<td><?= campo_data( 'stjdtconclusao', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão.' ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							3.3 anexar documento:
						</td>
						<td><input type="button" name="botao" value="Anexar arquivo" onclick="javascript:window.open('sisplan.php?modulo=principal/atividade_/anexo&acao=A&atiid='+<?=$atiid ?>+'&tptid=3', 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');"/></td>
					</tr>
			<?php // } ?>	
			<?php 
				if ( (verificasituacaocontratacao( $cttid, 'situacaojuridica', NAO_SE_APLICA )) || (verificasituacaocontratacao( $cttid, 'situacaojuridica', CONCLUIDA ))  ) { ?>
					<tr>
						<td align='left' colspan="2"><hr></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; bold;">
							<b>4. Situação da contratação:</b>
						</td>
						<td><?php
					$sql = sprintf( "SELECT stuid as codigo, studsc AS descricao FROM sisplan.situacao WHERE stustatus = 'A' ORDER BY 2" );	
					$db->monta_combo("stcidsituacao",$sql,$permissao_formulario,'&nbsp','','','','150','S','stcidsituacao');
						 ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.1 Início da contratação:
						</td>
						<td><?= campo_data( 'stcdtinicio', 'S', $permissao_formulario, '', 'S', 'Informe a data de ínicio da contratação.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.2 Data de conclusão prevista:
						</td>
						<td><?= campo_data( 'stcdtprevisao', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão prevista.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.3 Data de conclusão:
						</td>
						<td><?= campo_data( 'stcdtconclusao', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.4 Nota de empenho:
						</td>
						<td><?= campo_texto('stcnotaempenho','S','S','',20,12,'','','','','','id="stcnotaempenho"'); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.5 Número do Contrato:
						</td>
						<td><?= campo_texto('stcnumerocontrato','S','S','',20,12,'','','','','','id="stcnumerocontrato"'); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:middle; width:25%;">
							4.6 Publicação contratação:
						</td>
						<td align="center">
							<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
								<tr>
									<td align='right' class="SubTituloDireita" style="vertical-align:top; width:30%;">
										Data de publicação do resultado:
									</td>
									<td><?= campo_data( 'stcdtpublicacaoresult', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão.' ); ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita" style="vertical-align:top; width:30%;">
										Local de publicação:
									</td>
									<td><?php
								$sql = sprintf( "SELECT lcpid as codigo, lcpdsc AS descricao FROM sisplan.localpublicacao WHERE lcpstatus = 'A' ORDER BY 2" );
									$db->monta_combo("stcidlocalpubresult",$sql,$permissao_formulario,'&nbsp','','','','150','S','stcidlocalpubresult');
									 ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita" style="vertical-align:top; width:30%;">
										Data de publicação do extrato de contrato:
									</td>
									<td><?= campo_data( 'stcdtpublicacaoextrato', 'S', $permissao_formulario, '', 'S', 'Informe a data de conclusão.' ); ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita" style="vertical-align:top; width:30%;">
										Local de publicação:
									</td>
									<td><?php $sql = sprintf( "SELECT lcpid as codigo, lcpdsc AS descricao FROM sisplan.localpublicacao WHERE lcpstatus = 'A' ORDER BY 2" );
									$db->monta_combo("stcidlocalpubextrato", $sql,$permissao_formulario,'&nbsp','','','','150','S','stcidlocalpubextrato');
 									?></td>
								</tr>
							</table>

						</td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.7 Contratado:
						</td>
						<td>
							<?php
								if ( strlen(trim($stccontratado)) <= 11 ) 
									$stccontratado = formatar_cpf(trim($stccontratado));
								else 
									$stccontratado = formatar_cnpj(trim($stccontratado));
							?>
							<?= campo_texto( 'stccontratado', 'S', 'S', '', 25, 14, '###.###.###-##', '', 'right', '', 0, ' id="stccontratado" ' . $complemento ); ?>
						</td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.8 Contratado - Tipo:
						</td>
						<td><?
					$sql = sprintf( "select tpcid as codigo, tpcdsc as descricao from sisplan.tipocontratado where tpcstatus = 'A' order by 2 ");
				 
				$db->monta_combo("tpcid",$sql,$permissao_formulario,'&nbsp;','formata_campo_contratado','','','250','S','tpcid');
						?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.8 Valor contratado / empenhado:
						</td>
						<td>
							<?= campo_texto( 'stcvalorcontratado', 'S', 'S', '', 12, 14, '##.###.###.###,##', '', 'right', '', 0, ' id="stcvalorcontratado" onblur="MouseBlur(this);" ' . $complemento ); ?>
							<script> 	
								if (document.getElementById('stcvalorcontratado').value != '')
									document.getElementById('stcvalorcontratado').value = float2moeda(document.getElementById('stcvalorcontratado').value);
							</script>
							</td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							4.9 anexar documento:
						</td>
						<td><input type="button" name="botao" value="Anexar arquivo" onclick="javascript:window.open('sisplan.php?modulo=principal/atividade_/anexo&acao=A&atiid='+<?=$atiid ?>+'&tptid=4', 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');"/></td>
					</tr>

			<?php //} ?>	
			<?php 
				if ( (verificasituacaocontratacao( $cttid, 'situacaocontratacao', NAO_SE_APLICA) ) || (verificasituacaocontratacao( $cttid, 'situacaocontratacao', CONCLUIDA) ) ) { ?>

					<tr>
						<td align='left' colspan="2"><hr></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; bold;">
							<b>5. Dados da execução:</b>
						</td>
						<td><?php
						$sql = sprintf( "SELECT stuid as codigo, studsc AS descricao FROM sisplan.situacao WHERE stustatus = 'A' ORDER BY 2" );
						$db->monta_combo("steidsituacao",$sql,$permissao_formulario,'&nbsp','','','','150','S','steidsituacao');
						 ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							5.1 Data de início (contrato):
						</td>
						<td><?= campo_data( 'stedtinicio', 'S', $permissao_formulario, '', 'S', 'Informe a data de ínicio da execução.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							5.2 Data de término (contrato):
						</td>
						<td><?= campo_data( 'stedttermino', 'S', $permissao_formulario, '', 'S', 'Informe a data de término da execução.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							5.3 Número de parcelas / medições:
						</td>
						<td><?= campo_data( 'stenumparcelas', 'S', $permissao_formulario, '', 'S', 'Informe o número de parcelas.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							5.4 Prestação de contas:
						</td>
						<td><?php
						$sql = sprintf( "SELECT spcid as codigo, spcdsc AS descricao FROM sisplan.situacaoprestacaoconta WHERE spcstatus = 'A' ORDER BY 2" );
						$db->monta_combo("spcid",$sql,$permissao_formulario,'&nbsp','','','','150','S','spcid');
						 ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							5.5 Data da prestação de contas:
						</td>
						<td><?= campo_data( 'stedtprestacao', 'S', $permissao_formulario, '', 'S', 'Informe a data de prestação de contas.' ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							5.6 Anexar documento:
						</td>
						<td><input type="button" name="botao" value="Anexar arquivo" onclick="javascript:window.open('sisplan.php?modulo=principal/atividade_/anexo&acao=A&atiid='+<?=$atiid ?>+'&tptid=5', 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');"/></td>
					</tr>

			<?php //} ?>	



			<?php 
				if ( (verificasituacaocontratacao( $cttid, 'situacaoexecucao', NAO_SE_APLICA )) || (verificasituacaocontratacao( $cttid, 'situacaoexecucao', CONCLUIDA )) ) { ?>
					<tr>
						<td align='left' colspan="2"><hr></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; bold;">
							<b>6. Aditivo:</b>
						</td>
						<td><?php
					$sql = sprintf( "SELECT tpaid as codigo, tpadsc AS descricao FROM sisplan.tipoaditivo WHERE tpastatus = 'A' ORDER BY 2" );
						$db->monta_combo("tpaid",$sql,$permissao_formulario,'&nbsp','verifica_tipo_aditivo','','','150','S','tpaid');
						 ?></td>
					</tr> 
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							6.1 Data do aditivo:
						</td>
						<td><?= campo_data( 'saddtaditivo', 'S', $permissao_formulario, '', 'S', 'Informe a data de ínicio da execução.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							6.2 Valor do aditivo:
						</td>
						<td><? $complemento = ' onblur="calcula_percentual_aditivo(this.value);" '; ?>
							<?= campo_texto( 'sadvlraditivo', 'N', 'S', '', 25, 14, '##.###.###.###,##', '', 'right', '', 0, ' id="sadvlraditivo" ', null, null, "calcula_percentual_aditivo(this.value);" ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							6.3 % do total do contrato:
						</td>
						<td><? $complemento = ''; ?>
						<?= campo_texto( 'sadporcentocontrato', 'S', 'S', '', 25, 14, '##.###.###.###,##', '', 'right', '', 0, ' id="sadporcentocontrato" onblur="MouseBlur(this);" ' . $complemento ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							6.4 Nova data de término do contrato:
						</td>
						<td><?= campo_data( 'saddttermino', 'S', $permissao_formulario, '', 'S', 'Informe a data de término da execução.' ); ?></td>
					</tr>
					<tr> 
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							6.5 Justificativa:
						</td>
						<td><?= campo_textarea( 'sadjustificativa', 'S', $permissao_formulario, '', 70, 3, 500 ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							6.6 Anexar documento:
						</td>
						<td><input type="button" name="botao" value="Anexar arquivo" onclick="javascript:window.open('sisplan.php?modulo=principal/atividade_/anexo&acao=A&atiid='+<?=$atiid ?>+'&tptid=6', 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');"/>
					<?php if ( $sadid ) { ?>
						<input type="button" name="botaoad" value="Salvar " onclick="enviarAditivo();"/></td>
					<?php } ?>
					</tr>
					<tr>
						<td colspan="2" align="center">
						<?php
							$sql = "";
							$sql = "SELECT 
										'<center><img style=\"cursor:pointer;\" src=\"/imagens/consultar.gif\" border=\"0\" title=\"Editar aditivo\" onclick=\"EditarAditivo(' || sad.sadid || ')\"></center>' as acao,
										ta.tpadsc as tipo,
										to_char( saddtaditivo, 'DD/MM/YYYY') as dtaditivo,
										sadvlraditivo as vlraditivo
									FROM
										sisplan.situacaoaditivo sad
									INNER JOIN sisplan.tipoaditivo ta ON ta.tpaid = sad.tpaid
									WHERE
										sad.cttid = ".$cttid."";

							$cabecalho = array('Ações', 'Tipo de Aditivo', 'Data do Aditivo', 'Valor do aditivo');
							$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'S' );
						?>


					<?php // } ?>
					
								<?php } ?>
							<?php } ?>
						<?php } ?>
					<?php } ?>
				<?php } ?>	
						</td>
					</tr>



					<?php if( $permissao ): ?>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
						<?php if ( !$sadid ) { ?>
							<td><input type="button" name="botao" value="Salvar" onclick="enviar();"/></td>
						<?php } else { ?>
							<td>&nbsp;</td>
						<?php } ?>
						</tr>
					<?php endif; ?>
				</table>
			</form>
		</td>
	</tr>
</table>