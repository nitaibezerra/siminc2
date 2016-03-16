<?php 
//Teste XLS
if($_POST['limpaSession']){
	unset($_SESSION['evento']['post']);
	$_POST['limpaSession'] = "";
}

if( empty($_SESSION['evento']['post']) ){
	$_SESSION['evento']['post'] = $_POST;
}


//Rececendo os valores dos filtros
extract($_SESSION['evento']['post']);

// monta os elementos que vão ser recuperados
$select = array();
for($i=0; $i < count($_REQUEST["agrupador"]); $i++) {
	switch($_REQUEST["agrupador"][$i]) {
		
		//SELECT Entidade Contratada		
		case 'contratada':
			$select[$i] = " entidadeContratada.entnome AS empresaContratada ";
			break;
		//SELECT CNPJ Entidade Contratada	
		case 'cnpjContratada':
			$select[$i] = "entidadeContratada.entnumcpfcnpj AS CPNJ_Contratada";
			break;
		//SELECT Fiscal Titular da Entidade
		case 'fiscalTitular':
			$select[$i] = "entidadeFiscalTitular.entnome AS fiscalTitular ";
			break;
		//SELECT CPF Fiscal Titular da Entidade
		case 'cpfFiscalTitular':
			$select[$i] = "entidadeFiscalTitular.entnumcpfcnpj AS CPF_Fiscal_Titular";
			break;
		//SELECT Fiscal Substituto da Entidade
		case 'fiscalSubstituto':
			$select[$i] = "entidadeFiscalSubstituta.entnome  AS fiscalSubstituto ";
			break;
		//SELECT CPF Fiscal Substituto da Entidade
		case 'cpfFiscalSubstituto':
			$select[$i] = "entidadeFiscalSubstituta.entnumcpfcnpj AS CPF_Fiscal_Substituto";
			break;	
		//SELECT Área Responsável	
		case 'responsavel':
			$select[$i] = "unidadeAtendimento.unasigla As areaResponsavel";
			break;
		//SELECT Data de Vencimento do Contrato
		case 'vencimento':
			$select[$i] = "to_char(contrato.ctrdtfimvig, 'DD/MM/YYYY') AS dataVencimentoContrato";
			break;
		//SELECT Data inicio Modalidade Garantida	
		case 'dtInicioModalidadeGarantida':
			$select[$i] = "to_char(contrato.ctrdtinigar, 'DD/MM/YYYY') AS dataInicioModalidadeGarantida";
			break;
		//SELECT Data fim Modalidade Garantida
		case 'dtFimModalidadeGarantida':
			$select[$i] = "to_char(contrato.ctrdtinigar, 'DD/MM/YYYY')AS dataFimModalidadeGarantida";
			break;
		//SELECT numero do Processo Execução Financeira
		case 'n_processoexefin':
			$select[$i] = "contrato.ctrprocexecfin AS processoExecucaoFinanceira";
			break;
		//SELECT numero do Processo Execução do Contrato
		case 'n_processoexectr':
			$select[$i] = "contrato.ctrprocexecctr AS processoExecucaoContrato";
			break;
		//SELECT numero do Contrato
		case 'n_contrato':
			$select[$i] = "contrato.ctrnum AS numeroContrato";
			break;
		//SELECT tipo do Contrato
		case 'tipoContrato':
			$select[$i] = "tipoContrato.tpcdsc AS tipoContrato";
			break;
		//SELECT ano do Contrato
		case 'anoContrato':
			$select[$i] = "contrato.ctrano AS anoContrato";
			break;
		//SELECT modalidade
		case 'modalidade':
			$select[$i] = "modalidadeContrato.moddsc AS modalidadeDoContrato";
			break;
		//SELECT número da modalidade
		case 'n_modalidade':
			$select[$i] = "contrato.ctrnummod AS numeroModalidadeDoContrato";
			break;
		//SELECT Valor Inicial	
		case 'vlr_inicial':
			$select[$i] = "contrato.ctrvlrinicial AS valorInicial ";
			break;
		//SELECT Valor Mensal
		case 'vlr_mensal':
			$select[$i] = "contrato.ctrvlrmensal AS valorMensal";
			break;
		//SELECT Valor total
		case 'vlr_total':
			$select[$i] = "contrato.ctrvlrtotal AS valorTotal";
			break;
		//SELECT Valor Global
		case 'vlr_global':
			$select[$i] = "contrato.ctrvlrglobal AS valorGlobal ";
			break;
		//SELECT Modalidade Garantida
		case 'modalidadeGarantida':
			$select[$i] = "modalidadeGarantida.mogdsc AS modalidadeGarantida";
			break;
		//SELECT Objeto		
		case 'objeto':
			$select[$i] = "contrato.ctrobj AS objetoContrato";
			break;	
		//SELECT Situção
		case 'situacao':
			$select[$i] = "situacaoContrato.sitdsc AS situacaoContrato";
			break;
		//SELECT Observação Vigência
		case 'obv_vigencia':
			$select[$i] = "observacaoVigencia.obvdsc AS observacaoVigente";
			break;		
		//SELECT Número do Cronograma
		case 'num_cronograma':
			$select[$i] = "contrato.ctrnumcron AS numeroCronograma";
			break;		
		//SELECT Observação do Contrato
		case 'obv_contrato':
			$select[$i] = "contrato.ctrobs AS observacaoContrato";
			break;
		//SELECT Valor Modalidade Garantida
		case 'vlr_modalidadeGarantida':
			$select[$i] = "contrato.ctrvlrgarantia AS valorModalidadeGarantida";
			break;
		//SELECT Observação Modalidade Garantida
		case 'obv_modalidadeGarantida':
			$select[$i] = "contrato.ctrobsgar AS observacaoModalidadeGarantida";
			break;		
		//SELECT Número da Portaria do Fiscal
		case 'num_portaria_fiscal':
			$select[$i] = "contrato.ctnumportfiscal AS numeroPortariaFiscal";
			break;		
		//SELECT Data da Portaria do Fiscal
		case 'dt_portaria_fiscal':
			$select[$i] = "to_char(contrato.ctdataportfiscal, 'DD/MM/YYYY') AS dataPortariaFiscal";
			break;
		//SELECT Processo do Contrato
		case 'processo_contrato':
			$select[$i] = "contrato.ctrproccontr AS processoContrato ";
			break;	
	}
}

$select = implode(", ", $select);

//Entidade Contratada
if( $contratada[0] && ( $contratada_campo_flag || $contratada_campo_flag == '1' )){
	$where[0] = " AND entidadeContratada.entnome  " . (( $contratada_campo_excludente == null || $contratada_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('".implode("','",$contratada)."') ";		
}
//CNPJ Entidade Contratada
if(	$cnpjContratada[0]	&&	(	$cnpjContratada_campo_flag	||	$cnpjContratada_campo_flag	==	'1'	)	){
	$where[1]  =	"	AND	entidadeContratada.entnumcpfcnpj 	"	.	(	(	$cnpjContratada_campo_excludente == null	|| $cnpjContratada_campo_excludente == '0') ?'IN':'NOT IN')."(".implode	(",",$cnpjContratada)."')";
}
//Fiscal Titular da Entidade
if(	$fiscalTitular[0]	&&	(	$fiscalTitular_campo_flag	||	$fiscalTitular_campo_flag	==	'1'	)	){
	$where[2] =	"	AND	entidadeFiscalTitular.entid 	"	.	(	(	$fiscalTitular_campo_excludente == null	||	$fiscalTitular_campo_excludente == '0') ?'IN':'NOT IN')."(".implode(",",$fiscalTitular)."')";
}
//CPF Fiscal Titular da Entidade
if(	$cpfFiscalTitular[0]	&&	(	$cpfFiscalTitular_campo_flag	||	$cpfFiscalTitular_campo_flag	==	'1'	)	){
	$where[3] =	"	AND	entidadeFiscalTitular.entnumcpfcnpj	"	.	(	(	$cpfFiscalTitular_campo_excludente == null	||	$cpfFiscalTitular_campo_excludente == '0') ?'IN':'NOT IN')."(".implode(",",$cpfFiscalTitular)."')";
}
//Fiscal Substituto da Entidade
if(	$fiscalSubstituto[0]	&&	(	$fiscalSubstituto_campo_flag	||	$fiscalSubstituto_campo_flag	==	'1'	)	){
	$where[4] =	"	AND	entidadeFiscalSubstituta.entid	"	.	(	(	$fiscalSubstituto_campo_excludente == null	||	$fiscalSubstituto_campo_excludente == '0') ?'IN':'NOT IN')."(".implode(",",$fiscalSubstituto)."')";
}
//CPF Fiscal Substituto da Entidade
if(	$cpfFiscalSubstituto[0]	&&	(	$cpfFiscalSubstituto_campo_flag	||	$cpfFiscalSubstituto_campo_flag	==	'1'	)	){
	$where[5] =	"	AND	entidadeFiscalSubstituta.entnumcpfcnpj	"	.	(	(	$cpfFiscalSubstituto_campo_excludente == null	||	$cpfFiscalSubstituto_campo_excludente == '0') ?'IN':'NOT IN')."(".implode(",",$cpfFiscalSubstituto)."')";
}
//Área Reponsável
if( $responsavel[0] && ($responsavel_campo_flag || $responsavel_campo_flag == '1' )){
	$where[6] = " AND unidadeAtendimento.unasigla " . (( $responsavel_campo_excludente == null || $responsavel_campo_excludente == '0') ? ' IN ' : ' NOT IN ')." ('".implode( "','",$responsavel )."') ";		
}
//Data Vencimento dos Contrato
//if( $vencimento[0] && ( $vencimento_campo_flag || $vencimento_campo_flag == '1' )){
//	$where[7] = " AND contrato.ctrdtfimvig " . (( $vencimento_campo_excludente == null || $vencimento_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $vencimento ) . "') ";		
//}
//Data inicio Modalidade Garantida
if(	$dtInicioModalidadeGarantida[0]	&&	(	$dtInicioModalidadeGarantida_campo_flag	||	$dtInicioModalidadeGarantida_campo_flag	==	'1'	)	){
	$where[8] =	"	AND	contrato.ctrdtinigar	"	.	(	(	$dtInicioModalidadeGarantida_campo_excludente == null	||	$dtInicioModalidadeGarantida_campo_excludente == '0') ?'IN':'NOT IN')."(".implode(",",$dtInicioModalidadeGarantida)."')";
}
//Data fim Modalidade Garantida
if(	$dtFimModalidadeGarantida[0]	&&	(	$dtFimModalidadeGarantida_campo_flag	||	$dtFimModalidadeGarantida_campo_flag	==	'1'	)	){
	$where[9] =	"	AND	contrato.ctrdtfimgar	"	.	(	(	$dtFimModalidadeGarantida_campo_excludente == null	||	$dtFimModalidadeGarantida_campo_excludente == '0') ?'IN':'NOT IN')."(".implode(",",$dtFimModalidadeGarantida)."')";
}
//Processo Execução Financeira
if(	$n_processoexefin[0]	&&	(	$n_processoexefin_campo_flag	||	$n_processoexefin_campo_flag	==	'1'	)	){
	$where[10] =	"	AND	 contrato.ctrprocexecfin	"	.	(	(	$n_processoexefin_campo_excludente == null	||	$n_processoexefin_campo_excludente == '0')	?'IN':'NOT IN')."(".implode(",",$n_processoexefin)."')";
}
//Processo Execução do Contrato
if(	$n_processoexectr[0]	&&	(	$n_processoexectr_campo_flag	||	$n_processoexectr_campo_flag	==	'1'	)	){
	$where[11] =	"	AND contrato.ctrprocexecctr 	"	.	(	(	$n_processoexectr_campo_excludente == null	||	$n_processoexectr_campo_excludente == '0')	?'IN':'NOT IN')."(".implode(",",$n_processoexectr)."')";	
}
//Número do Contrato
if(	$n_contrato[0]	&&	(	$n_contrato_campo_flag	||	$n_contrato_campo_flag	==	'1'	)	){
	$where[12] =    "	AND	contrato.ctrnum 	"	.	(	(	$n_contrato_campo_excludente == null	||	$n_contrato_campo_excludente == '0'	)	?'IN':'NOT IN')."(".implode(",",$n_contrato	)."')";
}
//Tipo do Contrato
if(	$tipoContrato[0]	&&	(	$tipoContrato_campo_flag	||	$tipoContrato_campo_flag	==	'1'	)	){
	$where[13] = "AND tipoContrato.tpcdsc	".(($tipoContrato_campo_excludente == null	||	$tipoContrato_campo_excludente == '0')?'IN':'NOT IN')."('".implode("','",$tipoContrato)."')";
}
//Ano do Contrato
if(	$anoContrato[0]	&&	(	$anoContrato_campo_flag	||	$anoContrato_campo_flag	==	'1'	)	){
	$where[14] =	"	AND	 contrato.ctrano	"	.	(	(	$anoContrato_campo_excludente == null	||	$anoContrato_campo_excludente == '0')	?'IN':'NOT IN')."('".implode("','",$anoContrato)."')";
}
//Modalidade
if(	$modalidade[0]	&&	(	$modalidade_campo_flag	||	$modalidade_campo_flag	==	'1'	)	){
	$where[15] =	"	AND	modalidadeContrato.moddsc	"	.	(	(	$modalidade_campo_excludente == null	||	$modalidade_campo_excludente == '0')	?'IN':'NOT IN')."('".implode("','",$modalidade)."')";
}
//Número da Modalidade
if(	$n_modalidade[0]	&&	(	$n_modalidade_campo_flag	||	$n_modalidade_campo_flag	==	'1'	)	){
	$where[16] =	"	AND	 contrato.ctrnummod	"	.	(	(	$n_modalidade_campo_excludente == null	||	$n_modalidade_campo_excludente == '0')	?'IN':'NOT IN')."(".implode(",",$n_modalidade)."')";
}
// Valor Inicial
if(	$vlr_inicial[0]	&&	(	$vlr_inicial_campo_flag	||	$vlr_inicial_campo_flag	==	'1'	)	){
	$where[17] =	"	AND	contrato.ctrvlrinicial	"	.	(	(	$vlr_inicial_campo_excludente == null	||	$vlr_inicial_campo_excludente == '0')	?'IN':'NOT IN')."(".implode(",",$vlr_inicial)."')";
}
//Valor Mensal 
if(	$vlr_mensal[0]	&&	(	$vlr_mensal_campo_flag	||	$vlr_mensal_campo_flag	==	'1'	)	){
	$where[18] =	"	AND	contrato.ctrvlrmensal	"	.	(	(	$vlr_mensal_campo_excludente == null	||	$vlr_mensal_campo_excludente == '0')	?'IN':'NOT IN')."(".implode(",",$vlr_mensal)."')";
}
//Valor Total
if(	$vlr_total[0]	&&	(	$vlr_total_campo_flag	||	$vlr_total_campo_flag	==	'1'	)	){
	$where[19] =	"	AND	contrato.ctrvlrtotal	"	.	(	(	$vlr_total_campo_excludente == null	||	$vlr_total_campo_excludente == '0')	?'IN':'NOT IN')."(".implode(",",$vlr_total)."')";
}
//Valor Global
if(	$vlr_global[0]	&&	(	$vlr_global_campo_flag	||	$vlr_global_campo_flag	==	'1'	)	){
	$where[20] =	"	AND	 contrato.ctrvlrglobal 	"	.	(	(	$vlr_global_campo_excludente == null	||	$vlr_global_campo_excludente == '0')	?'IN':'NOT IN')."(".implode(",",$vlr_global)."')";
}
//Modalidade Garantida
if(	$modalidadeGarantida[0]	&&	(	$modalidadeGarantida_campo_flag	||	$modalidadeGarantida_campo_flag	==	'1'	)	){
	$where[21] =	"	AND	modalidadeGarantida.mogdsc 	"	.	(	(	$modalidadeGarantida_campo_excludente == null	||	$modalidadeGarantida_campo_excludente == '0')	?'IN':'NOT IN')."(".implode	(",",$modalidadeGarantida)."')";
}
//Objeto
if( $objeto[0] && ( $objeto_campo_flag || $objeto_campo_flag == '1' )){
	$where[22] = " AND contrato.ctrobj " . (( $objeto_campo_excludente == null || $objeto_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode( "','", $objeto )."')";		
}
//Situação
if( $situacao[0] && ( $situacao_campo_flag || $situacao_campo_flag == '1' )){
	$where[23] = " AND situacaoContrato.sitdsc " . (( $situacao_campo_excludente == null || $situacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode( "','",$situacao)."')";		
}
//Observação Vigência
if( $obv_vigencia[0] && ( $obv_vigencia_campo_flag || $obv_vigencia_campo_flag == '1' )){
	$where[24] = " AND observacaoVigencia.obvdsc " . (( $obv_vigencia_campo_excludente == null || $obv_vigencia_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode("','",$obv_vigencia)."')";		
}
//Número Cronograma
if( $num_cronograma[0] && ( $num_cronograma_campo_flag || $num_cronograma_campo_flag == '1' )){
	$where[25] = " AND contrato.ctrnumcron " . (( $num_cronograma_campo_excludente == null || $num_cronograma_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode("','",$num_cronograma)."')";		
}
//Observação Contrato
if( $obv_contrato[0] && ( $obv_contrato_campo_flag || $obv_contrato_campo_flag == '1' )){
	$where[26] = " AND contrato.ctrobs " . (( $obv_contrato_campo_excludente == null || $obv_contrato_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode( "','", $obv_contrato). "')";		
}
//Valor Modalidade Garantida
if( $vlr_modalidadeGarantida[0] && ( $vlr_modalidadeGarantida_campo_flag || $vlr_modalidadeGarantida_campo_flag == '1' )){
	$where[27] = " AND contrato.ctrvlrgarantia " . (( $vlr_modalidadeGarantida_campo_excludente == null || $vlr_modalidadeGarantida_campo_excludente == '0') ? ' IN ' : ' NOT IN')."('".implode("','",$vlr_modalidadeGarantida)."')";		
}
//Observação Modalidade Garantida
if( $obv_modalidadeGarantida[0] && ( $obv_modalidadeGarantida_campo_flag || $obv_modalidadeGarantida_campo_flag == '1' )){
	$where[28] = " AND contrato.ctrobsgar " . (( $obv_modalidadeGarantida_campo_excludente == null || $obv_modalidadeGarantida_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode("','",$obv_modalidadeGarantida)."')";		
}
//Número da Portaria do Fiscal
if( $num_portaria_fiscal[0] && ( $num_portaria_fiscal_campo_flag || $num_portaria_fiscal_campo_flag == '1' )){
	$where[29] = " AND contrato.ctnumportfiscal " . (( $num_portaria_fiscal_campo_excludente == null || $num_portaria_fiscal_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode("','",$num_portaria_fiscal)."')";		
}
//Data da Portaria do Fiscal
if( $dt_portaria_fiscal[0] && ( $dt_portaria_fiscal_campo_flag || $dt_portaria_fiscal_campo_flag == '1' )){
	$where[30] = " AND contrato.ctdataportfiscal " . (( $dt_portaria_fiscal_campo_excludente == null || $dt_portaria_fiscal_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode("','", $dt_portaria_fiscal)."')";		
}
//Processo Contrato
if( $processo_contrato[0] && ( $processo_contrato_campo_flag || $processo_contrato_campo_flag == '1' )){
	$where[31] = " AND contrato.ctrproccontr " . (( $processo_contrato_campo_excludente == null || $processo_contrato_campo_excludente == '0') ? ' IN ' : ' NOT IN ')."('".implode("','", $processo_contrato )."')";		
}

//Campo de Data referente ao Vencimento dos Contratos
if( $dtinicio[0] && ( $dtinicio_campo_flag || $dtinicio_campo_flag == '1' )){
	$where[32] = " AND contrato.ctrdtfimvig " . (( $dtinicio_campo_excludente == null || $dtinicio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dtinicio ) . "') ";		
}
if( $dtfim[0] && ($dtfim_campo_flag || $dtfim_campo_flag == '1' )){
	$where[33] = " AND contrato.ctrdtfimvig " . (( $dtfim_campo_excludente == null || $dtfim_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dtfim ) . "') ";
}
//Revertendo o formato da Data de dd/mm/aaaa, para aaaa/mm/dd 
if( $_POST['dtinicio'] && $_POST['dtfim'] ){
	$novadtinicio = substr($_POST['dtinicio'],6,4).'/'.substr($_POST['dtinicio'],3,2).'/'.substr($_POST['dtinicio'],0,2);
	$novadtfim = substr($_POST['dtfim'],6,4).'/'.substr($_POST['dtfim'],3,2).'/'.substr($_POST['dtfim'],0,2);
//Condição que é utilizada, quando as datas de inicio e fim forem selecionadas. 	
	$where[34] = "AND contrato.ctrdtfimvig BETWEEN '{$novadtinicio}'	AND	'{$novadtfim}'";
}
///////////////////////////////////////////////////////////SELECT DA CONSULTA DO RELATÓRIO GERAL DE CONTRATOS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(is_array($where)){
	if(count($where)> 0){
		$listaWhrew = implode( " ", $where );
	}else{
		$listaWhrew = '';
	}
}
$sql  = "	SELECT 
				".$select."
			 FROM 
				evento.ctcontrato AS contrato
			LEFT JOIN 
				entidade.entidade AS entidadeContratada ON contrato.entidcontratada = entidadeContratada.entid 
			LEFT JOIN
				entidade.entidade AS entidadeFiscalTitular ON contrato.entidfiscaltit = entidadeFiscalTitular.entid 
			LEFT JOIN
				entidade.entidade AS entidadeFiscalSubstituta ON contrato.entidfiscalsub = entidadeFiscalSubstituta.entid
			LEFT JOIN
				demandas.unidadeatendimento AS unidadeAtendimento ON contrato.unaid = unidadeAtendimento.unaid
			LEFT JOIN
				evento.cttipocontrato AS tipoContrato ON contrato.tpcid = tipoContrato.tpcid
			LEFT JOIN
				evento.ctmodalidadecontrato AS modalidadeContrato ON contrato.modid = modalidadeContrato.modid
			LEFT JOIN
				evento.ctmodalidadegarantia AS modalidadeGarantida ON contrato.mogid = modalidadeGarantida.mogid
			LEFT JOIN
				evento.ctsituacaocontrato AS situacaoContrato ON contrato.sitid = situacaoContrato.sitid
			LEFT JOIN
				evento.ctobsvigencia AS observacaoVigencia ON contrato.obvid = observacaoVigencia.obvid
			WHERE
				unidadeAtendimento.unasigla = unidadeAtendimento.unasigla		 			  
				".$listaWhrew."
				
		";


//ver($where);
//ver($listaWhrew);
//ver($responsavel);
//ver($where[6]);
//ver($where); 
//dbg($arDados);
//ver($sql);
//ver($where[34],$novadtfim,$novadtinicio);
$n_proceso = '';
$unidades = array();

$arrCol = $_SESSION['evento']['post']['agrupador'];
foreach( $_SESSION['evento']['post']['agrupador'] as $coluna ){
	//Cabeçalho das Colunas
	if($coluna == "contratada"){ $cabecalho[] = "Contratada"; }
	if($coluna == "cnpjContratada"){$cabecalho[] = "CNPJ Contratada";}
	if($coluna == "fiscalTitular"){$cabecalho[] =	"Fiscal Titular"; }
	if($coluna == "cpfFiscalTitular"){$cabecalho[] = "CPF Fiscal Titular";}
	if($coluna == "fiscalSubstituto"){$cabecalho[]	= "Fiscal Substituto ";}
	if($coluna == "cpfFiscalSubstituto"){$cabecalho[] = "CPF Fiscal Substituto ";}
	if($coluna == "responsavel"){$cabecalho[] = "Área Responsável";}
	if($coluna == "vencimento"){$cabecalho[] = "Vencimento"; }
	if($coluna == "dtInicioModalidadeGarantida"){$cabecalho[] = "Data Inicio Modalidade Garantida";}
	if($coluna == "dtFimModalidadeGarantida"){$cabecalho[] = "Data Fim Modalidade Garantida ";}
	if($coluna == "n_processoexefin"){$cabecalho[] = "Processo Execução Financeira";}
	if($coluna == "n_processoexectr"){$cabecalho[] = "Processo Execução do Contrato";}
	if($coluna == "n_contrato"){$cabecalho[] = "Número";}
	if($coluna == "tipoContrato"){$cabecalho[] = "Tipo do Contrato";}
	if($coluna == "anoContrato"){$cabecalho[] = "Ano ";}
	if($coluna == "modalidade"){$cabecalho[] = "Modalidade";}
	if($coluna == "n_modalidade"){$cabecalho[] = "Número Modalidade ";}
	if($coluna == "vlr_inicial"){$cabecalho[] = "Valor Inicial";}
	if($coluna == "vlr_mensal"){$cabecalho[] = "Valor Atual Mensal";}
	if($coluna == "vlr_total"){$cabecalho[] = "Valor Atual Anual";}
	if($coluna == "vlr_global"){$cabecalho[] = "Valor Atual Global";}
	if($coluna == "modalidadeGarantida"){$cabecalho[] = "Modalidade Garantida";}
	if($coluna == "objeto"){$cabecalho[] = "Objeto";}
	if($coluna == "situacao"){$cabecalho[] = "Situação da Vigência";}
	if($coluna == "obv_vigencia"){$cabecalho[] = "Observação da Vigência";}
	if($coluna == "num_cronograma"){$cabecalho[] = "Número do Cronograma";}
	if($coluna == "obv_contrato"){$cabecalho[] = "Observação do Contrato";}
	if($coluna == "vlr_modalidadeGarantida"){$cabecalho[] = "Valor Modalidade Garantida";}
	if($coluna == "obv_modalidadeGarantida"){$cabecalho[] = "Observação Modalidade Garantida";}
	if($coluna == "num_portaria_fiscal"){$cabecalho[] = "Número da Portaria do Fiscal";}
	if($coluna == "dt_portaria_fiscal"){$cabecalho[] = "Data da Portaria do Fiscal";}
	if($coluna == "processo_contrato"){$cabecalho[] = "Processo do Contrato";}
}
if($_REQUEST['consulta'] == 'xls'){
	header('content-type: text/html; charset=ISO-8859-1');
	$db->sql_to_excel($sql, 'RelatorioContratos', $cabecalho);
	echo "<script>window.parent.close();</script>";
	exit;
}
$arDados = $db->carregar( $sql );
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<table style="width:100%;">
	<tr>	
		<td>
		<?php 
		echo "<br/>";
		$cabecalhoBrasao = monta_cabecalho_relatorio('100');
		echo $cabecalhoBrasao;
		print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">';
		print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Relatório Geral de Contratos</label></td></tr><tr>';
		print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >&nbsp;</td></tr></table>';
		?>		
		</td>
	</tr>
	<tr>
		<td id="listaArray">
		<?php 
		$db->monta_lista_array($arDados, $cabecalho, 1000000, 1, 'N', 'center', '');													
		?>		
		</td>
	</tr>
</table>
<script>
var td = document.getElementById('listaArray');
var table = td.getElementsByTagName('table')[1];
table.style.width = '100%';
</script>
</body>
</html>