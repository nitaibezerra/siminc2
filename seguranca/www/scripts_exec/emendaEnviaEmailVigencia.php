<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

include_once "/var/www/simec/global/config.inc";

//carrega as funções gerais
//include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '00000000191';

#Fluxo das Emendas  Convênios
conveniosParlamentar();
conveniosBeneficiarioEmenda();
conveniosElaborarPlanoTrabalho();
//conveniosRealizarAnalisePTA();

#Fluxo das Emendas do PAR
emendaParParlamentar();
emendaParBeneficiario();
emendaParLiberarPropostaTrabalho();
emendaParAceitarProposta();
emendaParIncluirPropostaSubacao();
//emendaParRealizarAnaliseTecnica();

#Fluxo das Emendas Descentralização
emendaDescentralizacaoParlamentar();
emendaDescentralizacaoPropostaPTA();
//emendaDescentralizacaoAnaliseTecncia();

#Fluxo das Emendas Universidades/Institutos Federais
emendaUniversidadeInstitutoParlamentar();


function conveniosParlamentar(){
	
	$dataini = date('Y').'-02-03';
	$datafim = date('Y').'-02-18';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
					e.emeid,
				    e.emecod,
				    case when a.autemail is not null then u.usuemail end as emaildep
				FROM
					emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
					inner join emenda.usuarioresponsabilidade ur on ur.autid = a.autid and ur.rpustatus = 'A'
				    left join seguranca.usuario u on u.usucpf = ur.usucpf
				WHERE
					ur.pflcod = 295
				    and e.emeano = '".date('Y')."'
				    and e.etoid = 3
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and e.emeid not in (select ed.emeid from emenda.emendadetalhe ed
				                    	inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid where ede.edestatus = 'A')
				order by emaildep";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['emaildep']][] = $v['emecod'];
		}
		//ver($arrEmail,d);
		$strEmailTo = array();
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Indicação do beneficiário da Emenda Convênio';
			array_push($strEmailTo, $email);
			
			$strMensagem = "Senhor(a) Parlamentar,<br>O Ministério da Educação informa que a emenda (".implode(', ', $arEmenda)."), alocada nesta pasta, <br>
							encontra-se disponível para detalhamento dos objetos da emenda, seus beneficiários e valores destinados. <br>
							A indicação deverá ser realizada no módulo EMENDAS/SIMEC, até 18/02/".date('Y').".";
			
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>E-mail</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['emaildep'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function conveniosBeneficiarioEmenda(){
	
	$dataini = date('Y').'-02-18';
	$datafim = date('Y').'-03-03';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    e.resid,
				    ede.edemailresp,
				    ede.edenomerep,
				    ede.edecpfresp
				FROM
					emenda.emenda e
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				WHERE
					e.emeano = '".date('Y')."'
				    and e.etoid = 3
				    and ede.edestatus = 'A'
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and ede.edecpfresp not in (select distinct pu.usucpf from seguranca.usuario u 
				    							inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				    							inner join seguranca.perfil p on p.pflcod = pu.pflcod 
				                                where p.sisid = 57 and p.pflcod = 274 and u.usustatus = 'A')
				order by edemailresp";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['resid']][] = array('emecod' => $v['emecod'], 'beneficiario' => $v['edecpfresp'].' - '.$v['edenomerep']);
		}
		
		$arBenef = array();
		$arEmecod = array();
		$strAssunto 	= 'Liberar acesso PAR/SIMEC para beneficiário da Emenda';
		foreach ($arrEmail as $resid => $arEmenda) {
			
			foreach ($arEmenda as $v) {
				if( !in_array( $v['beneficiario'], $arBenef) ) array_push($arBenef, $v['beneficiario']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
			}
			
			$strEmailTo = array();
			$strMensagem = '';
			if( $resid == '1' ){
				$strEmailTo  = array( $_SESSION['email_sistema'] );
				$strMensagem = "Equipe técnica da SESU,<br>favor liberar acesso ao Módulo de Emendas/SIMEC para o(s) beneficiário(s) abaixo:<br>".implode(', <br>', $arBenef)."<br>da(s) Emenda(s) (".implode(', ', $arEmecod)."), até 03/03/".date('Y').".";
				enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
			} elseif( $resid == '3' ){
				$strEmailTo  = array( $_SESSION['email_sistema'] );
				$strMensagem = "Equipe técnica do FNDE,<br>favor liberar acesso ao Módulo de Emendas/SIMEC para o(s) beneficiário(s) abaixo:<br>".implode(', <br>', $arBenef)."<br>da(s) Emenda(s) (".implode(', ', $arEmecod)."), até 03/03/".date('Y').".";
				enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
			}			
			$arBenef = array();
			$arEmecod = array();
		}
	}
	if( $arrDados[0] ){
		$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<thead>
					<tr>
						<td><strong>Emenda</strong></td>
						<td><strong>E-mail</strong></td>
						<td><strong>Beneficiario</strong></td>
					</tr>
				</thead>
				<tbody>';
			foreach ($arrDados as $v) {
				$strMensagem.= '
					<tr>
						<td>'.$v['emecod'].'</td>
						<td>'.$v['edemailresp'].'</td>
						<td>'.$v['edenomerep'].'</td>
					</tr>';
			}
				$strMensagem.= '
				</tbody>
				</table>';
		
		$strEmailTo = array($_SESSION['email_sistema']);
		return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
	}
}

function conveniosElaborarPlanoTrabalho(){
	$dataini = date('Y').'-02-23';
	$datafim = date('Y').'-03-19';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    ede.edemailresp,
    				a.autnome,
				    eb.enbnome
				FROM
				    emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and e.etoid = 3
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and ede.enbid not in (select enbid from emenda.planotrabalho where ptrstatus = 'A' and ptrexercicio = '".date('Y')."' and sisid = 57)
				order by edemailresp";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		//ver($arrDados,d);
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['edemailresp']][] = array('emecod' => $v['emecod'], 'parlamentar' => $v['autnome'], 'entidade' => $v['enbnome']);
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Cadastrar proposta do plano de trabalho';
			$strEmailTo 	= $email;
			
			$arParlamentar = array();
			$arEntidade = array();
			$arEmecod = array();
			foreach ($arEmenda as $v) {
				if( !in_array( $v['parlamentar'], $arParlamentar) ) array_push($arParlamentar, $v['parlamentar']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
				if( !in_array( $v['entidade'], $arEntidade) ) array_push($arEntidade, $v['entidade']);
			}
			
			$strMensagem = "O Ministério da Educação informa que a entidade (".implode(', ', $arEntidade).") foi indicada como beneficiária da emenda (".implode(', ', $arEmecod).") do parlamentar (".implode(', ', $arParlamentar)."). <br>
							O prazo para aceite da emenda e cadastro do plano de trabalho será até 19/03/".date('Y')." e deverá ser realizada no módulo EMENDAS/SIMEC.";
			//ver($strMensagem);
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>parlamentar</strong></td>
							<td><strong>entidade</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['autnome'].'</td>
							<td>'.$v['enbnome'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function conveniosRealizarAnalisePTA(){
	$dataini = date('Y').'-03-20';
	$datafim = date('Y').'-04-19';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    ede.edemailresp,
				    res.resdsc,
				    usu.usuemail
				FROM
				    emenda.emenda e
				    inner join emenda.responsavel res on res.resid = e.resid
				    inner join emenda.usuarioresponsabilidade ur on ur.resid = e.resid and ur.rpustatus = 'A' --and ur.pflcod = ''
				    inner join seguranca.usuario usu on usu.usucpf = ur.usucpf and usu.usustatus = 'A'
				    inner join seguranca.usuario_sistema us on us.usucpf = usu.usucpf and us.sisid = 57 and us.suscod = 'A'
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.ptemendadetalheentidade pte on pte.edeid = ede.edeid
				    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid
				    inner join workflow.documento doc on doc.docid = ptr.docid
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and e.etoid = 3
				    and doc.esdid in (54, 56)
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and ptr.ptrid not in (select a.ptrid from emenda.analise a 
				                            where a.anastatus = 'A' and a.anatipo = 'T' 
				                            and a.anadataconclusao is not null
				                            and a.analote = (select max(analote) from emenda.analise where anatipo = 'T' and ptrid = a.ptrid))
				order by edemailresp";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['edemailresp']][] = array('emecod' => $v['emecod'], 'parlamentar' => $v['autnome'], 'entidade' => $v['enbnome']);
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Realizar Análise Mérito';
			$strEmailTo 	= $email;
			
			$arParlamentar = array();
			$arEntidade = array();
			$arEmecod = array();
			foreach ($arEmenda as $v) {
				if( !in_array( $v['parlamentar'], $arParlamentar) ) array_push($arParlamentar, $v['parlamentar']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
				if( !in_array( $v['entidade'], $arEntidade) ) array_push($arEntidade, $v['entidade']);
			}
			
			$strMensagem = "Equipe técnica da xxx favor providenciar análise do plano de trabalho xxx, no Módulo de Emendas/SIMEC, até 19/04/".date('Y').".";
			
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>parlamentar</strong></td>
							<td><strong>entidade</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['autnome'].'</td>
							<td>'.$v['enbnome'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaParParlamentar(){
	
	$dataini = date('Y').'-02-03';
	$datafim = date('Y').'-02-18';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emeid,
				    e.emecod,
				    case when a.autemail is not null then u.usuemail end as emaildep
				FROM
				    emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.usuarioresponsabilidade ur on ur.autid = a.autid and ur.rpustatus = 'A'
				    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid and vf.prgano = '".date('Y')."' and vf.acastatus = 'A'
				    left join seguranca.usuario u on u.usucpf = ur.usucpf
				WHERE
				    ur.pflcod = 295
				    and e.emeano = '".date('Y')."'
				    and vf.unicod = '26298'
				    and e.etoid = 1
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and e.emeid not in (select ed.emeid from emenda.emendadetalhe ed
				                        inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid where ede.edestatus = 'A')
				order by emaildep";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['emaildep']][] = $v['emecod'];
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Indicação do beneficiário da Emenda PAR';
			$strEmailTo 	= $email;
			$strMensagem = "Senhor(a) Parlamentar,<br>o Ministério da Educação informa que a(s) emenda(s) (".implode(', ', $arEmenda)."), alocada nesta pasta, <br>
							encontra-se disponível para detalhamento dos objetos da emenda, seus beneficiários e valores destinados. <br>
							A indicação deverá ser efetuada no módulo EMENDAS/SIMEC até 18/02/".date('Y').".";
			
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>e-mail</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['emaildep'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaParBeneficiario(){
	
	$dataini = date('Y').'-02-13';
	$datafim = date('Y').'-02-21';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    e.resid,
				    ede.edemailresp,
				    ede.edenomerep,
				    ede.edecpfresp
				FROM
					emenda.emenda e
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				WHERE
					e.emeano = '".date('Y')."'
				    and e.etoid = 1
				    and ede.edestatus = 'A'
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and ede.edecpfresp not in (select distinct pu.usucpf from seguranca.usuario u 
				    							inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				    							inner join seguranca.perfil p on p.pflcod = pu.pflcod 
				                                where p.sisid = 57 and p.pflcod = 274 and u.usustatus = 'A')
				order by edemailresp";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['resid']][] = array('emecod' => $v['emecod'], 'beneficiario' => $v['edecpfresp'].' - '.$v['edenomerep']);
		}
		//ver($arrEmail,d);
		$arBenef = array();
		$arEmecod = array();
		$strAssunto 	= 'Liberar acesso PAR/SIMEC para beneficiário da Emenda';
		foreach ($arrEmail as $resid => $arEmenda) {
			
			foreach ($arEmenda as $v) {
				if( !in_array( $v['beneficiario'], $arBenef) ) array_push($arBenef, $v['beneficiario']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
			}
			
			$strEmailTo = array();
			$strMensagem = '';
			if( $resid == '1' ){
				$strEmailTo  = array( $_SESSION['email_sistema'] );
				$strMensagem = "Equipe técnica da SESU,<br>favor liberar acesso ao Módulo de Emendas/SIMEC para o(s) beneficiário(s) abaixo:<br>".implode(', <br>', $arBenef)."<br>da(s) Emenda(s) (".implode(', ', $arEmecod)."), até 21/02/".date('Y').".";
				enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
			} elseif( $resid == '3' ){
				$strEmailTo  = array( $_SESSION['email_sistema'] );
				$strMensagem = "Equipe técnica do FNDE,<br>favor liberar acesso ao Módulo de Emendas/SIMEC para o(s) beneficiário(s) abaixo:<br>".implode(', <br>', $arBenef)."<br>da(s) Emenda(s) (".implode(', ', $arEmecod)."), até 21/02/".date('Y').".";
				enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
			}			
			$arBenef = array();
			$arEmecod = array();
		}
		
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>responsavel</strong></td>
							<td><strong>e-mail</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['edemailresp'].'</td>
							<td>'.$v['edemailresp'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaParLiberarPropostaTrabalho(){
	$dataini = date('Y').'-02-19';
	$datafim = date('Y').'-02-26';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    e.resid,
				    ede.edemailresp,
				    eb.enbnome,
				    eb.enbcnpj
				FROM
				    emenda.emenda e
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and e.etoid = 1
                    and ede.ededisponivelpta = 'N'
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				order by edemailresp";
		
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['resid']][] = array('emecod' => $v['emecod'], 'entidade' => $v['enbcnpj'].' - '.$v['enbnome']);
		}
		
		foreach ($arrEmail as $resid => $arEmenda) {
			
			$arEntidade = array();
			$arEmecod = array();
			foreach ($arEmenda as $v) {
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
				if( !in_array( $v['entidade'], $arEntidade) ) array_push($arEntidade, $v['entidade']);
			}
			$strEmailTo = array();
			$strMensagem 	= '';
			$strAssunto 	= 'Liberar Proposta de Trabalho para beneficiário da Emenda PAR';
			if( $resid == '1' ){
				$strEmailTo  = array( $_SESSION['email_sistema'] );
				$strMensagem = "Equipe técnica da SESU favor disponibilizar Proposta de Trabalho para beneficiário (".implode(',<br>', $arEntidade).") da Emenda (".implode(', ', $arEmecod)."). O prazo para disponibilização é até  26/02".date('Y');
				enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
			} elseif( $resid == '3' ){
				$strEmailTo  = array( $_SESSION['email_sistema'] );
				$strMensagem = "Equipe técnica do FNDE favor disponibilizar Proposta de Trabalho para beneficiário (".implode(',<br>', $arEntidade).") da Emenda (".implode(', ', $arEmecod)."). O prazo para disponibilização é até  26/02".date('Y');
				enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
			}			
		}
		
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>entidade</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['enbcnpj'].' - '.$v['enbnome'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaParAceitarProposta(){
	$dataini = date('Y').'-02-24';
	$datafim = date('Y').'-03-05';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    ede.edemailresp,
    				a.autnome,
				    eb.enbnome
				FROM
				    emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and e.etoid = 1
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
                    and ede.edeid in ( select edeid from emenda.emendapariniciativa)
				    and ede.enbid not in (select enbid from emenda.planotrabalho where ptrstatus = 'A' and ptrexercicio = '".date('Y')."' and sisid = 23)
				order by edemailresp";
		
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['edemailresp']][] = array('emecod' => $v['emecod'], 'autnome' => $v['autnome'], 'enbnome' => $v['enbnome']);
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Aceitar proposta da Emenda';
			$strEmailTo 	= $email;
			
			$arParlamentar = array();
			$arEntidade = array();
			$arEmecod = array();
			foreach ($arEmenda as $v) {
				if( !in_array( $v['autnome'], $arParlamentar) ) array_push($arParlamentar, $v['autnome']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
				if( !in_array( $v['enbnome'], $arEntidade) ) array_push($arEntidade, $v['enbnome']);
			}
			
			$strMensagem = "O Ministério da Educação informa que a entidade (".implode(', ', $arEntidade).") foi indicada como beneficiária da emenda (".implode(', ', $arEmecod).") do parlamentar (".implode(', ', $arParlamentar)."). <br>
							O aceite da emenda deverá ser realizado no módulo PAR/SIMEC, até 05/03".date('Y');
			
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>entidade</strong></td>
							<td><strong>Autor</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['enbnome'].'</td>
							<td>'.$v['autnome'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaParIncluirPropostaSubacao(){
	$dataini = date('Y').'-02-24';
	$datafim = date('Y').'-03-05';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    ede.edemailresp,
    				a.autnome,
				    eb.enbnome
				FROM
				    emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and e.etoid = 1
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
                    and ede.edeid in ( select edeid from emenda.emendapariniciativa)
				    and ede.enbid in (select enbid from emenda.planotrabalho where ptrstatus = 'A' and ptrexercicio = '".date('Y')."' and sisid = 23)
                    and ed.emdid not in (select emdid from par.subacaoemendapta)
				order by edemailresp";
		
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['edemailresp']][] = array('emecod' => $v['emecod'], 'autnome' => $v['autnome'], 'enbnome' => $v['enbnome']);
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Incluir Proposta de Trabalho';
			$strEmailTo 	= $email;
			
			$arParlamentar = array();
			$arEntidade = array();
			$arEmecod = array();
			foreach ($arEmenda as $v) {
				if( !in_array( $v['autnome'], $arParlamentar) ) array_push($arParlamentar, $v['autnome']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
				if( !in_array( $v['enbnome'], $arEntidade) ) array_push($arEntidade, $v['enbnome']);
			}
			
			$strMensagem = "O Ministério da Educação informa que a inclusão da proposta da emenda (".implode(', ', $arEmecod).") do parlamentar (".implode(', ', $arParlamentar).") deverá ser realizada no módulo PAR/SIMEC, até 05/03/".date('Y');
			
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>entidade</strong></td>
							<td><strong>Autor</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['enbnome'].'</td>
							<td>'.$v['autnome'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaParRealizarAnaliseTecnica(){
	$dataini = date('Y').'-03-06';
	$datafim = date('Y').'-03-20';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod
				FROM
				    emenda.emenda e
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and e.etoid = 1
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
                    and ede.edeid in ( select edeid from emenda.emendapariniciativa)
				    and ede.enbid in (select enbid from emenda.planotrabalho where ptrstatus = 'A' and ptrexercicio = '".date('Y')."' and sisid = 23)
                    and ed.emdid in (select se.emdid from par.subacaoemendapta se
                                        inner join par.subacaodetalhe sd on sd.sbdid = se.sbdid and sd.sbdano = '".date('Y')."'
                                    where sd.sbdparecer is null)";
		$arrDados = $db->carregarColuna($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		//ver($arrEmail,d);
		$strAssunto = 'Realizar Análise Técnica';
		$strMensagem = "Equipe técnica do FNDE, o prazo para análise da proposta da emenda (".implode(', ', $arrDados).") é até  20/03/".date('Y').".";
		enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		
		$strEmailTo = array($_SESSION['email_sistema']);				
		return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
	}
}

function emendaDescentralizacaoParlamentar(){
	
	$dataini = date('Y').'-02-03';
	$datafim = date('Y').'-02-18';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emeid,
				    e.emecod,
				    case when a.autemail is not null then u.usuemail end as emaildep
				FROM
				    emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.usuarioresponsabilidade ur on ur.autid = a.autid and ur.rpustatus = 'A'
				    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid and vf.prgano = '".date('Y')."' and vf.acastatus = 'A'
				    left join seguranca.usuario u on u.usucpf = ur.usucpf
				WHERE
				    ur.pflcod = 295
				    and e.emeano = '".date('Y')."'
				    and vf.unicod = '26101'
				    and e.etoid = 2
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and e.emeid not in (select ed.emeid from emenda.emendadetalhe ed
				                        inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid where ede.edestatus = 'A')
				order by emaildep";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['emaildep']][] = $v['emecod'];
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Indicação do beneficiário da Emenda Descentralização';
			$strEmailTo 	= $email;
			$strMensagem = "Senhor(a) Parlamentar,<br>O Ministério da Educação informa que a emenda (".implode(', ', $arEmenda)."), alocada nesta pasta, <br>
							encontra-se disponível para detalhamento dos objetos da emenda, seus beneficiários e valores destinados. <br>
							A indicação deverá ser realizada no módulo EMENDAS/SIMEC, até 18/02/".date('Y').".";
			//ver($strMensagem);
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>e-mail</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['emaildep'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaDescentralizacaoPropostaPTA(){
	$dataini = date('Y').'-02-13';
	$datafim = date('Y').'-02-28';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    ede.edemailresp,
    				a.autnome,
				    eb.enbnome
				FROM
				    emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid
				    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid and vf.prgano = '".date('Y')."' and vf.acastatus = 'A'
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and vf.unicod = '26101'
				    and e.etoid = 2
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and ede.enbid not in (select enbid from emenda.planotrabalho where ptrstatus = 'A' and ptrexercicio = '".date('Y')."' and sisid = 57)
				order by edemailresp";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['edemailresp']][] = array('emecod' => $v['emecod'], 'parlamentar' => $v['autnome'], 'entidade' => $v['enbnome']);
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Cadastrar proposta do plano de trabalho (Termo de cooperação)';
			$strEmailTo 	= $email;
			
			$arParlamentar = array();
			$arEntidade = array();
			$arEmecod = array();
			foreach ($arEmenda as $v) {
				if( !in_array( $v['parlamentar'], $arParlamentar) ) array_push($arParlamentar, $v['parlamentar']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
				if( !in_array( $v['entidade'], $arEntidade) ) array_push($arEntidade, $v['entidade']);
			}
			
			$strMensagem = "A instituição (".implode(', ', $arEntidade).") foi indicada como beneficiária da emenda (".implode(', ', $arEmecod).") do parlamentar (".implode(', ', $arParlamentar)."). A apresentação do respectivo termo de cooperação deverá ser realizada no módulo de Termos de Cooperação do SIMEC até 28/02".date('Y').".";
			//ver($strMensagem);
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>parlamentar</strong></td>
							<td><strong>entidade</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['autnome'].'</td>
							<td>'.$v['enbnome'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function emendaDescentralizacaoAnaliseTecncia(){
	$dataini = date('Y').'-03-10';
	$datafim = date('Y').'-03-20';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));

	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emecod,
				    ede.edemailresp,
    				a.autnome,
				    eb.enbnome
				FROM
				    emenda.emenda e
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid
				    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid and vf.prgano = '".date('Y')."' and vf.acastatus = 'A'
				WHERE
				    e.emeano = '".date('Y')."'
				    and ede.edestatus = 'A'
				    and e.etoid = 2
				    and vf.unicod = '26101'
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and ede.enbid not in (select enbid from emenda.planotrabalho where ptrstatus = 'A' and ptrexercicio = '".date('Y')."' and sisid = 57)
				order by edemailresp";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['edemailresp']][] = array('emecod' => $v['emecod'], 'parlamentar' => $v['autnome'], 'entidade' => $v['enbnome']);
		}
		//ver($arrEmail,d);
		$arParlamentar = array();
		$arEntidade = array();
		$arEmecod = array();
		foreach ($arrEmail as $email => $arEmenda) {
			
			foreach ($arEmenda as $v) {
				if( !in_array( $v['parlamentar'], $arParlamentar) ) array_push($arParlamentar, $v['parlamentar']);
				if( !in_array( $v['emecod'], $arEmecod) ) array_push($arEmecod, $v['emecod']);
				if( !in_array( $v['entidade'], $arEntidade) ) array_push($arEntidade, $v['entidade']);
			}
		}
		$strAssunto 	= 'Cadastrar proposta do plano de trabalho (Termo de cooperação)';
		$strEmailTo 	= array($_SESSION['email_sistema']);
		$strMensagem = "Termo de cooperação xxx foi cadastrado no módulo de Termos de Cooperação no SIMEC.<br> 
							Equipe técnica da xxx favor providenciar análise do plano de trabalho xxx, no Módulo de Emendas/SIMEC, até 20/03/".date('Y').".";
		enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		
		$strEmailTo = array($_SESSION['email_sistema']);				
		return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
	}
}

function emendaUniversidadeInstitutoParlamentar(){
	
	$dataini = date('Y').'-02-03';
	$datafim = date('Y').'-03-31';
	
	$dataini = strtotime($dataini);
	$datafim = strtotime($datafim);
	$data = strtotime(date('Y-m-d'));
	
	if( $dataini <= $data && $datafim >= $data ){
		$db = new cls_banco();
		
		$sql = "SELECT distinct
				    e.emeid,
				    e.emecod,
                    ede.edemailresp 
				FROM
				    emenda.emenda e
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
                    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
				    inner join emenda.usuarioresponsabilidade ur on ur.usucpf = ede.edecpfresp and ur.rpustatus = 'A'
				    left join seguranca.usuario u on u.usucpf = ur.usucpf
				WHERE
				    ur.pflcod = 274
				    and e.emeano = '".date('Y')."'
				    and e.etoid = 4
				    and ede.edestatus = 'A'
				    and e.emeid in (select distinct emeid from emenda.emendadetalhe where emdimpositiva = 6)
				    and ede.edeid not in (select pt.edeid from emenda.ptemendadetalheentidade pt
				                        inner join emenda.planotrabalho ptr on ptr.ptrid = pt.ptrid where ptr.ptrexercicio = '".date('Y')."')";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrEmail = array();
		foreach ($arrDados as $v) {
			$arrEmail[$v['edemailresp']][] = $v['emecod'];
		}
		//ver($arrEmail,d);
		foreach ($arrEmail as $email => $arEmenda) {
			$strAssunto 	= 'Cadastrar proposta simplificada da execução da emenda Universidades/Institutos Federais';
			$strEmailTo 	= $email;
			$strMensagem = "Prezado Reitor,<br>o Ministério da Educação informa que a emenda (".implode(', ', $arEmenda)."), alocada nesta UO, encontra-se disponível para cadastramento da proposta simplificada da execução da emenda ou indicação da inviabilidade de execução até 31/03/".date('Y').".";
			//ver($strMensagem);
			enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
		if( $arrDados[0] ){
			$strMensagem = '<table width="100%" border="1" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<thead>
						<tr>
							<td><strong>Emenda</strong></td>
							<td><strong>e-mail</strong></td>
						</tr>
					</thead>
					<tbody>';
				foreach ($arrDados as $v) {
					$strMensagem.= '
						<tr>
							<td>'.$v['emecod'].'</td>
							<td>'.$v['edemailresp'].'</td>
						</tr>';
				}
					$strMensagem.= '
					</tbody>
					</table>';
			
			$strEmailTo = array($_SESSION['email_sistema']);				
			return enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo);
		}
	}
}

function enviaEmailVigencia($strAssunto, $strMensagem, $strEmailTo){
		
	
	if( $strEmailTo ){		
		$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
		//ver($remetente, $strEmailTo, $strAssunto, $strMensagem,d);				
		$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
	} else {
		return true;
	}
}

?>
