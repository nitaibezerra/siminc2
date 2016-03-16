<?php

switch( $_REQUEST['evento'] ){

	case 'carregar':

		$diretorio  = APPRAIZ.'arquivos/sigplan/';

		$xml = simplexml_load_file( $diretorio.$_REQUEST['arquivo'] );
		
		$insert_programa = array();
		$insert_acao     = array();

		
		/*
		$create_programa = "
		CREATE TABLE carga.programa
		(
		prgano character(4),
		prgcod character(4),
		tpocod character(1),
		orgcod character(5),
		prgdsc character varying(200),
		tprcod character(1),
		prgdscobjetivo text,
		prgdscpublicoalvo text,
		prgdscjustificativa text,
		prgdscestrategia text,
		prgvlrprograma double precision,
		prgvlrppa double precision,
		prgsntemporario boolean,
		sitcodestagio character(2),
		sitcodandamento character(2),
		sitcodcronograma character(2)
		)";


		$create_acao = "
		CREATE TABLE carga.acao
		(
		prgano character(4),
		prgcod character(4),
		acacod character(4),
		saccod character(4),
		loccod character(4),
		regcod character(2),
		taccod character(1),
		procod integer,
		unmcod integer,
		unicod character(5),
		unitpocod character(1),
		sitcodestagio character(2),
		sitcodandamento character(2),
		sitcodcronograma character(2),
		acadsc character varying(255),
		sacdsc character varying(300),
		acasnmedireta boolean,
		acasnmedesc boolean,
		acasnmelincred boolean,
		acasnmetanaocumulativa boolean,
		acavlrrealateanoanterior double precision,
		acasnrap boolean,
		acasnfiscalseguridade boolean,
		acasninvestatais boolean,
		acasnoutrasfontes boolean,
		cod_referencia bigint
		)";
		*/
		
		
		
		/*
         *			
		 * LIMPA AS TABELAS DE CARGA.ACAO E CARGA.PROGRAMA
		 *
		*/
		$sql = "TRUNCATE carga.acao, carga.programa";
		
		$db->executar($sql);
		

		/*
         *			
		 * INSERE OS PROGRAMAS NA TABELA DE CARGA.PROGRAMA
		 *
		*/
		foreach($xml->ArrayOfPrograma as $programas)
		{
			foreach ( $programas->Programa as $programa ) 
			{
				$db->executar("INSERT INTO carga.programa (prgano ,
														  prgcod ,
														  tpocod ,
														  orgcod ,
														  prgdsc ,
														  tprcod ,
														  prgdscobjetivo ,
														  prgdscpublicoalvo ,
														  prgdscjustificativa ,
														  prgdscestrategia ,
														  prgvlrprograma ,
														  prgvlrppa ,
														  prgsntemporario ,
														  sitcodestagio ,
														  sitcodandamento ,
														  sitcodcronograma 
														  ) 
														  VALUES 
														   ('". utf8_decode($programa->PRGAno) ."',
															'". utf8_decode($programa->PRGCod) ."',
															'". utf8_decode($programa->TPOCod) ."',
															'". utf8_decode($programa->ORGCod) ."',
															'". utf8_decode($programa->PRGDsc) ."',
															'". utf8_decode($programa->TPRCod) ."',
															'". utf8_decode($programa->PRGDscObjetivo) ."',
															'". utf8_decode($programa->PRGDscPublicoAlvo) ."',
															'". utf8_decode($programa->PRGDscJustificativa) ."',
															'". utf8_decode($programa->PRGDscEstrategia) ."',
															'". utf8_decode($programa->PRGVlrPrograma) ."',
															'". utf8_decode($programa->PRGVlrPPA) ."',
															'". utf8_decode($programa->PRGSNTemporario) ."',
															'". utf8_decode($programa->SITCodEstagio) ."',
															'". utf8_decode($programa->SITCodAndamento) ."',
															'". utf8_decode($programa->SITCodCronograma) ."'
														   )");
			}
		}

		
		

		/*
         *			
		 * INSERE AS AÇÕES NA TABELA DE CARGA.ACAO
		 *
		*/
		foreach($xml->ArrayOfAcao as $acoes)
		{
			foreach ( $acoes->Acao as $acao ) 
			{
				$db->executar("INSERT INTO carga.acao (prgano ,
													  prgcod ,
													  acacod ,
													  saccod ,
													  loccod ,
													  regcod ,
													  taccod ,
													  procod ,
													  unmcod ,
													  unicod ,
													  unitpocod ,
													  sitcodestagio ,
													  sitcodandamento ,
													  sitcodcronograma ,
													  acadsc ,
													  sacdsc ,
													  acasnmedireta ,
													  acasnmedesc ,
													  acasnmelincred ,
													  acasnmetanaocumulativa ,
													  acavlrrealateanoanterior ,
													  acasnrap ,
													  acasnfiscalseguridade ,
													  acasninvestatais ,
													  acasnoutrasfontes ,
													  cod_referencia 
													  ) 
													  VALUES 
													   ('". utf8_decode($acao->PRGAno) ."',
														'". utf8_decode($acao->PRGCod) ."',
														'". utf8_decode($acao->ACACod) ."',
														'". utf8_decode($acao->SACCod) ."',
														'". utf8_decode($acao->LOCCod) ."',
														'". utf8_decode($acao->REGCOD) ."',
														'". utf8_decode($acao->TACCod) ."',
														'". utf8_decode($acao->PROCod) ."',
														'". utf8_decode($acao->UNMCod) ."',
														'". utf8_decode($acao->UNICod) ."',
														'". utf8_decode($acao->UNITPOCod) ."',
														'". utf8_decode($acao->SITCodEstagio) ."',
														'". utf8_decode($acao->SITCodAndamento) ."',
														'". utf8_decode($acao->SITCodCronograma) ."',
														'". utf8_decode($acao->ACADsc) ."',
														'". utf8_decode($acao->SACDsc) ."',
														'". utf8_decode($acao->ACASNMEDireta) ."',
														'". utf8_decode($acao->ACASNMEDesc) ."',
														'". utf8_decode($acao->ACASNMELinCred) ."',
														'". utf8_decode($acao->ACASNMetaNaoCumulativa) ."',
														'". utf8_decode($acao->ACAVlrRealAteAnoAnterior) ."',
														'". utf8_decode($acao->ACASNRap) ."',
														'". utf8_decode($acao->ACASNFiscalSeguridade) ."',
														'". utf8_decode($acao->ACASNInvEstatais) ."',
														'". utf8_decode($acao->ACASNOutrasFontes) ."',
														'". utf8_decode($acao->COD_REFERENCIA) ."'
													   )");
			}
		}
		
		
		
		
		
		/*
         *			
		 * INSERE OS NOVOS PROGRAMAS NA TABELA MONITORA.PROGRAMA
		 *
		*/
		$sql = "
			INSERT INTO 
				monitora.programa ( prgano,
									prgcod,
									tpocod,
									orgcod,
									prgdsc,
									tprcod,
									prgdscobjetivo,
									prgdscpublicoalvo,
									prgdscjustificativa,
									prgdscestrategia,
									prgvlrprograma,
									prgvlrppa,
									prgsntemporario,
									sitcodestagio,
									sitcodandamento,
									sitcodcronograma) 
									( select 
										prgano,
										prgcod,
										tpocod,
										orgcod,
										prgdsc,
										tprcod,
										prgdscobjetivo,
										prgdscpublicoalvo,
										prgdscjustificativa,
										prgdscestrategia,
										prgvlrprograma,
										prgvlrppa,
										prgsntemporario,
										sitcodestagio,
										sitcodandamento,
										sitcodcronograma
									from 
										carga.programa t1 
									where not exists ( select 
												1 
											   from 
												monitora.programa t2 
											   where t2.prgcod = t1.prgcod 
											     and t2.prgano = t1.prgano 
											     and t2.prgstatus = 'A')
									)";
		$db->executar($sql);

		
		
		

		/*
         *			
		 * INSERE AS NOVAS AÇÕES NA TABELA MONITORA.PROGRAMA
		 *
		*/
		$sql = "
			INSERT INTO 
			    monitora.acao ( prgano,
								prgcod,
								prgid,
								acacod,
								saccod,
								loccod,
								regcod,
								taccod,
								procod,
								unmcod,
								unicod,
								unitpocod,
								sitcodestagio,
								sitcodandamento,
								sitcodcronograma,
								acadsc,
								sacdsc,
								acasnmedireta,
								acasnmedesc,
								acasnmelincred,
								acasnmetanaocumulativa,
								acavlrrealateanoanterior,
								acasnrap,
								acasnfiscalseguridade,
								acasninvestatais,
								acasnoutrasfontes,
								cod_referencia) 
								(select 
									prgano,
									prgcod,
									( select prgid from monitora.programa where prgcod = t1.prgcod and prgano = t1.prgano and prgstatus = 'A' ),
									acacod,
									saccod,
									loccod,
									regcod,
									taccod,
									procod,
									unmcod,
									unicod,
									unitpocod,
									sitcodestagio,
									sitcodandamento,
									sitcodcronograma,
									acadsc,
									sacdsc,
									acasnmedireta,
									acasnmedesc,
									acasnmelincred,
									acasnmetanaocumulativa,
									acavlrrealateanoanterior,
									acasnrap,
									acasnfiscalseguridade,
									acasninvestatais,
									acasnoutrasfontes,
									cod_referencia
								from 
									carga.acao t1 
								where not exists ( select 
										1 
									   from 
										monitora.acao t2 
									   where t2.prgcod = t1.prgcod 
									     and t2.prgano = t1.prgano
									     and t2.acacod = t1.acacod
									     and t2.loccod = t1.loccod
									     and t2.saccod = t1.saccod
									     and t2.sacdsc = t1.sacdsc
									     and t2.acasnrap = t1.acasnrap
									     and t2.unicod = t1.unicod
									     and t2.unitpocod = t1.unitpocod
									     and t2.acastatus = 'A'
									  )
								)";
		//dbg($sql,1);
		
		$db->executar($sql);
		
		
		$db->commit();

		break;
}


// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Comunicação SIGPLAN', 'Receber Dados' );
?>


<script language="javascript" type="text/javascript">
	
	function carregar_controle()
	{
		if ( validar_formulario_controle() ) 
		{
			if ( confirm('Deseja realmente iniciar o ingresso de dados a partir do arquivo selecionado?') )
			{
				document.controle.evento.value = 'carregar';
				document.controle.submit();
			}
		}
	}
	
	function validar_formulario_controle()
	{
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		
		
		if ( document.controle.arquivo.value == '' ) 
		{
			mensagem += '\nArquivo';
			validacao = false;
		}
		
		if ( !validacao ) 
			alert( mensagem );

		return validacao;
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

</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td>
			<!-- NOVO CONTROLE -->
				<form method="post" name="controle">
					<input type="hidden" name="evento" value=""/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Escolha um arquivo:</td>
							<td>
							<?
								$diretorio  = opendir(APPRAIZ.'arquivos/sigplan/');
								
								$itens = array();
								$x = 0;
								
								while ( $lista = readdir($diretorio) ) 
								{
									if (($lista!=".") && ($lista!=".."))
									{
        								$itens[$x]['descricao'] = $lista;
        								$itens[$x]['codigo'] = $lista;
        								$x++;
									}
								}
								
								$db->monta_combo( 'arquivo', $itens, 'S', '&nbsp;', '', '' );
							?>
							<?=obrigatorio()?>
							</td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td>
							<input type="button" name="botao" value="Carregar" onclick="carregar_controle();"/>
							</td>
						</tr>
					</table>
				</form>
		</td>
	</tr>
</table>