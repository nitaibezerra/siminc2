<?php

function verifica_tipo_autor_pa_descentralizado($atiid)
{
	global $db;

	$sql = sprintf("SELECT
						count(*)
					FROM
						pde.atividade a
					INNER JOIN seguranca.perfilusuario pu
						ON pu.usucpf = a.usucpf
					WHERE
						a.atiid = %s
					AND pu.pflcod in (%s, %s)", $atiid, PERFIL_TECNICO_UNIDADE_DESCENTRALIZADA, PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA);

	if( (int)$db->pegaUm( $sql, 0 ) <= 0 )
		return false;
	else
		return true;
}

function wf_prioridadePreenchida($pplid)
{
	global $db;

	$sql = sprintf("SELECT
						pplprioridade
					FROM
						sisplan.projetoplanejamento
					WHERE
						pplid = %s", (int) $pplid);

	if( $db->pegaUm( $sql) != '' )
		return true;
	else
		return false;
}


function verifica_tipo_autor_pa_centralizado( $atiid )
{
	global $db;

	$sql = sprintf("SELECT
						count(*)
					FROM
						pde.atividade a
					INNER JOIN seguranca.perfilusuario pu
						ON pu.usucpf = a.usucpf
					WHERE
						a.atiid = %s
					AND pu.pflcod not in (%s, %s)", $atiid, PERFIL_TECNICO_UNIDADE_DESCENTRALIZADA, PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA);

	if( (int)$db->pegaUm( $sql, 0 ) <= 0 )
		return false;
	else
		return true;
}


function verifica_pi_nao_existente( $atiid )
{
	global $db;

	$sql = sprintf("SELECT COUNT(atinumeropi) FROM pde.atividade a WHERE a.atiid = %s", $atiid);

	if( (int)$db->pegaUm( $sql, 0 ) <= 0 )
		return true;
	else
		return false;
}


function solicita_pi_minc( $atiid )
{
	// NOTE: debug com arquivo porque a requisicao vai acontecer em background
	// no workflow
	//
	/*
	 $fp = fopen("c:\teste.txt", "w+");
	 fputs( $fp, 'teste ' . $atiid );
	 fclose( $fp );
	*/
	return toXmlMinc($atiid, 'Atividade');
}

function cancelar_pi_minc ( $atiid )
{
	global $db;
	$sql = sprintf("UPDATE pde.atividade SET atistatus = 'I' WHERE atiid = %d",$atiid);
	$db->executar($sql);

	$result = toXmlMinc( $atiid, 'Atividade' );

	$sql = sprintf("UPDATE pde.atividade SET atistatus = 'A' WHERE atiid = %d",$atiid);
	$db->executar($sql);

	$db->commit();

	return $result;
}

function verifica_pi_enviado_minc ( $atiid )
{
	global $db;
	$sql = "SELECT
				aedid
			FROM pde.atividade a
			INNER JOIN workflow.historicodocumento hd
				ON a.docid = hd.docid
			WHERE aedid IN (". ACAO_ESTADO_DOC_ENVIADO .",". ACAO_ESTADO_DOC_CANCELADO.")
			  AND a.atiid = ". $atiid ."
			  AND hd.htddata = (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid IN (". ACAO_ESTADO_DOC_ENVIADO .",". ACAO_ESTADO_DOC_CANCELADO.") )";

	$eadid = (int)$db->pegaUm( $sql, 0 );

	if ( $eadid == ACAO_ESTADO_DOC_ENVIADO )
		return false;
	else
		return  true;

}


function sisplan_pegarDocid( $atiid )
{
    global $db;
    $sql = sprintf( "SELECT docid FROM pde.atividade WHERE atiid = %d", $atiid );
    $docid = $db->pegaUm( $sql );
    if( ! $docid )
	{
        $tpdid = WF_TIPODOC_PLANOACAO;
        //MONTA NOME DO DOC
        $sqlDescricao = sprintf( "SELECT a.* FROM pde.atividade a WHERE a.atiid = %d", $atiid );
        $linha = $db->pegaLinha( $sqlDescricao );
        $docdsc = $linha['atidescricao'] . "\n PAID: " . $linha['atiid'];
        // cria documento
        $docid = wf_cadastrarDocumento( $tpdid, $docdsc );
        $sql = sprintf( "UPDATE pde.atividade SET docid = %d WHERE atiid = %d", $docid, $atiid );
        $db->executar( $sql );
        $db->commit();
    }
    return ($docid);
}

function montaTabelaEmail($atiid, $txt, $mostraDados=true)
{
	global $db;
	$dadosPA = array();

	$sql = sprintf( "
	select
		distinct a.atiid, u.usunome, u.usuemail, '(' || u.usufoneddd || ') ' || u.usufonenum as telefone, o.orgcod || ' - ' || o.orgdsc as orgao,
		un.unicod || ' - ' || un.unidsc as unidade,
		x.uexcod || ' - ' || x.uexdsc as unidadeexecutora,
		atinumeropi,
		atianopi,
		a.atistatuspi,
		a.atijustificativarecusa,
		a.acaid,
		pduid,
		udaid,
		a.atipronac,
		a.atiprotocolo,
		a.atidescricao,
		a.atidetalhamento,
		a.atipropameta,
		a.atiprosecundariometa,
		a.atinumeroinstrumento,
		to_char( a.atidatainicio, 'DD/MM/YYYY') as atidatainicio,
		to_char( a.atidatafim, 'DD/MM/YYYY') as atidatafim,
		to_char( a.atidataprestacao, 'DD/MM/YYYY') as atidataprestacao,
		a.atiproponente,
		a.atirepresentante,
		a.atiemailrepresentante,
		a.atiorcamentocusteio as atiorcamentocusteio,
		a.atiorcamentocapital as atiorcamentocapital,
		a.atiorcamento as atiorcamento,
		dep.depdsc as departamento,
		dfe.dfedsc as desafio,
		ite.itedsc as iniciativa,
		pre.predsc as programaEstrategico,
		aes.aesdsc as acaoEstrategica,
		pdd.pdddsc as prioridade,
		cls.clsdsc as classe,
		sbc.sbcdsc as subclasse,
		are.aredsc as area,
		seg.segdsc as segmento,
		a.medlatitude as latitude,
		a.medlongitude as longitude,
		a.atipronacquest,
		a.atiemenda,
		stp.stpdsc as situacaocontratacao,
		itr.itrdsc as instrumentocontratacao,
		a.atinotacredito
	from
		seguranca.usuario	u
		join pde.atividade a on a.usucpf = u.usucpf
		left join public.orgao o on o.orgcod = u.orgcod
		left join public.unidade un on un.unicod = u.unicod
		left join planointerno.unidadeexecutora x on x.uexid = u.uexid
		LEFT JOIN sisplan.departamento dep ON dep.depid = a.depid
		LEFT JOIN sisplan.desafioestrategico dfe ON dfe.dfeid = a.dfeid
		LEFT JOIN sisplan.iniciativaestrategica ite ON ite.iteid = a.iteid
		LEFT JOIN planointerno.programaestrategico pre ON pre.preid = a.preid
		LEFT JOIN planointerno.acaoestrategica aes ON aes.aesid = a.aesid
		LEFT JOIN sisplan.prioridade pdd ON pdd.pddid = a.pddid
		LEFT JOIN sisplan.classe cls ON cls.clsid = a.clsid
		LEFT JOIN sisplan.subclasse sbc ON sbc.sbcid = a.sbcid
		LEFT JOIN planointerno.area are ON are.areid = a.areid
		LEFT JOIN planointerno.segmento seg ON seg.segid = a.segid
		LEFT JOIN sisplan.situacaoprocesso stp ON stp.stpid = a.stpid
		LEFT JOIN sisplan.instrumento itr ON itr.itrid = a.itrid
	where
		a.atiid = %d", $atiid );

	$dado = $db->pegaLinha($sql);
	extract($dado);

	$dadosPA = array();

/////////////////////////////////PROJETO////////////////////////////////////////
	array_push($dadosPA, array(
						 "label" => "ID",
						 "valor" => $atiid)
		  );
	if ( $atistatuspi == 'R' )
	{
		array_push($dadosPA, array(
							 "label" => "Motivo da Recusa",
							 "valor" => $atijustificativarecusa)
			  );
	}
	array_push($dadosPA, array(
							 "label" => "Solicitado por",
							 "valor" => $usunome)
			  );
	array_push($dadosPA, array(
							 "label" => "E-mail",
							 "valor" => $usuemail)
			  );
	array_push($dadosPA, array(
							 "label" => "Telefone",
							 "valor" => $telefone)
			  );
	if ( $atiprotocolo ){
		array_push($dadosPA, array(
								 "label" => "Número do Processo IPHAN",
								 "valor" => $atiprotocolo)
				  );
	}
	if ( $atinumeropi ){
		array_push($dadosPA, array(
								 "label" => "Número do PI",
								 "valor" => $atinumeropi)
				  );
	}
	if ( $atinotacredito ){
		array_push($dadosPA, array(
								 "label" => "Nota de Crédito",
								 "valor" => $atinotacredito)
				  );
	}
	array_push($dadosPA, array(
							 "label" => "Título",
							 "valor" => $atidescricao)
			  );
	array_push($dadosPA, array(
							 "label" => "Descrição",
							 "valor" => $atidetalhamento)
			  );
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////PPA////////////////////////////////////////
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => 'PPA')
			  );

	$dados = $db->pegaLinha( sprintf( "SELECT
											acacod,
											acadsc AS acao,
											a.prgcod,
											prgdsc AS programa,
											a.unicod,
											unidsc AS unidorc,
											a.loccod,
											locdsc AS localizador
										FROM
											monitora.acao a
											INNER JOIN monitora.programa p ON p.prgcod = a.prgcod
											INNER JOIN public.unidade u ON u.unicod = a.unicod
											INNER JOIN public.localizador l ON l.loccod = a.loccod
										WHERE
											a.acaid = %d"
									, $acaid
									)
								);
	extract($dados);

	array_push($dadosPA, array(
							 "label" => "Programa",
							 "valor" => $prgcod . " - " . $programa)
			  );
	array_push($dadosPA, array(
							 "label" => "Ação",
							 "valor" => $acacod . " - " . $acao)
			  );
	array_push($dadosPA, array(
							 "label" => "Unidade Orçamentária",
							 "valor" => $unicod . " - " . $unidorc)
			  );
	array_push($dadosPA, array(
							 "label" => "Localizador",
							 "valor" => $loccod . " - " . $localizador)
			  );

		$sql = sprintf( "SELECT
							p.procod AS codigo,
							p.prodsc AS produto,
							u.unmdsc AS unidade
						FROM
							public.produto p
							JOIN monitora.acao a ON p.procod = a.procod
							JOIN public.unidademedida u ON u.unmcod = a.unmcod
						WHERE
							a.acaid = '%s' AND
							p.prostatus = 'A'
						ORDER BY 2",
						$acaid);

		$dados = $db->pegaLinha($sql);
		extract($dados);

	array_push($dadosPA, array(
							 "label" => "Produto PPA",
							 "valor" => $produto)
			  );
	array_push($dadosPA, array(
							 "label" => "Unidade Medida",
							 "valor" => $unidade)
			  );
	array_push($dadosPA, array(
							 "label" => "Meta",
							 "valor" => $atipropameta)
			  );
	$produtoSec = $db->pegaUm("SELECT pdudsc FROM planointerno.produto WHERE pduid = '" . $pduid . "'");
	$unidMed = $db->pegaUm("SELECT udadsc FROM planointerno.unidademedida WHERE udaid = '" . $udaid . "'");

	array_push($dadosPA, array(
							 "label" => "Produto Secundário",
							 "valor" => $produtoSec)
			  );
	array_push($dadosPA, array(
							 "label" => "Unidade Medida",
							 "valor" => $unidMed)
			  );
	array_push($dadosPA, array(
							 "label" => "Meta",
							 "valor" => $atiprosecundariometa)
			  );
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////DADOS DA GESTÃO////////////////////////////////////////
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => 'Dados da Gestão')
			  );
	array_push($dadosPA, array(
							 "label" => "Unidade Executora",
							 "valor" => $unidadeexecutora)
			  );
	array_push($dadosPA, array(
							 "label" => "Departamento",
							 "valor" => $departamento)
			  );
/*
	array_push($dadosPA, array(
							 "label" => "Coordenação-Geral",
							 "valor" => $usunomecoordenacao)
			  );
*/
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////PRIORIDADE////////////////////////////////////////
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "Prioridade")
			  );
	array_push($dadosPA, array(
							 "label" => "Desafio Estratégico",
							 "valor" => $desafio)
			  );
	array_push($dadosPA, array(
							 "label" => "Iniciativa estratégica",
							 "valor" => $iniciativa)
			  );
	array_push($dadosPA, array(
							 "label" => "Programas estratégicos",
							 "valor" => $programaestrategico)
			  );
	array_push($dadosPA, array(
							 "label" => "Ações estratégicas",
							 "valor" => $acaoestrategica)
			  );
	/*
	array_push($dadosPA, array(
							 "label" => "Prioridade",
							 "valor" => $prioridade)
			  );
	*/
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////CARACTERIZAÇÃO DO PROJETO////////////////////////////////////////
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => 'Caracterização do Projeto')
			  );
	array_push($dadosPA, array(
							 "label" => "Classe",
							 "valor" => $classe)
			  );
	array_push($dadosPA, array(
							 "label" => "Sub-classe",
							 "valor" => $subclasse)
			  );
	array_push($dadosPA, array(
							 "label" => "Área",
							 "valor" => $area)
			  );
	array_push($dadosPA, array(
							 "label" => "Segmento",
							 "valor" => $segmento)
			  );
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////REGIONALIZAÇÃO////////////////////////////////////////
	$sql = "SELECT
				e.esfdsc as esfera,
				p.paidescricao as pais,
				es.estuf || ' - ' ||es.estdescricao as estuf,
				CASE
					WHEN es.estuf = 'DF' THEN ra.rgadsc
					ELSE m.mundescricao
				END AS municipio,
				pr.predsc,
				ac.aesdsc as acao,
				ue.uexdsc as unidadeexecutora,
				ar.aredsc as area,
				sg.segdsc as segmento,
				tu.taudsc as tipoautor,
				pt.ppodsc as partido,
				at.auedsc as autor,
				tp.tpidsc as tipo,
				tpp.tppdsc as tipoproponente,
				tsc.tscdsc as tiposetorcultural
			FROM
				pde.atividade a
				LEFT JOIN planointerno.regiaoadministrativa ra ON ra.rgaid = a.rgaid AND ra.rgastatus = 'A'
				LEFT JOIN planointerno.esfera e ON e.esfid = a.esfid
				LEFT JOIN territorios.pais p ON p.paiid = a.paiid
				LEFT JOIN territorios.estado es ON es.estuf = a.estuf
				LEFT JOIN territorios.municipio m ON m.muncod = a.muncod
				LEFT JOIN planointerno.programaestrategico pr ON pr.preid = a.preid
				LEFT JOIN planointerno.acaoestrategica ac ON ac.aesid = a.aesid
				LEFT JOIN planointerno.unidadeexecutora ue ON ue.uexid = a.uexid
				LEFT JOIN planointerno.area ar ON ar.areid = a.areid
				LEFT JOIN planointerno.segmento sg ON sg.segid = a.segid
				LEFT JOIN planointerno.tipoautor tu ON tu.tauid = a.tauid
				LEFT JOIN planointerno.partidopolitico pt ON pt.ppoid = a.ppoid
				LEFT JOIN planointerno.autoremenda at ON at.aueid = a.aueid
				LEFT JOIN planointerno.tipoinstrumento tp ON tp.tpiid = a.tpiid
				LEFT JOIN planointerno.tipoproponente tpp ON tpp.tppid = a.tppid
				LEFT JOIN planointerno.tiposetorcultural tsc ON tsc.tscid = a.tscid
			WHERE
			 	atiid = " . $atiid;
	$dado = $db->pegaLinha($sql);
	extract($dado);

	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "Regionalização")
			  );
	array_push($dadosPA, array(
							 "label" => "Esfera Administrativa",
							 "valor" => $esfera)
			  );
	array_push($dadosPA, array(
							 "label" => "Pais",
							 "valor" => $pais)
			  );
	array_push($dadosPA, array(
							 "label" => "Unidade Federativa",
							 "valor" => $estuf)
			  );
	array_push($dadosPA, array(
							 "label" => "Município",
							 "valor" => $municipio)
			  );
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////LOCALIZAÇÃO GEOGRÁFICA DA AÇÃO////////////////////////////////////////
	$latitude = explode(".", $latitude);
		$graulatitude = trim($latitude[0]) ? $latitude[0] : 0;
		$minlatitude = trim($latitude[1]) ? $latitude[1] : 0;
		$seglatitude = trim($latitude[2]) ? $latitude[2] : 0;
		$pololatitude = trim($latitude[3]) ? $latitude[3] : 0;
	$longitude = explode(".", $longitude);
		$graulongitude = trim($longitude[0]) ? $longitude[0] : 0;
		$minlongitude = trim($longitude[1]) ? $longitude[1] : 0;
		$seglongitude = trim($longitude[2]) ? $longitude[2] : 0;
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "Localização Geográfica da Ação")
			  );
	array_push($dadosPA, array(
							 "label" => "Latitude",
							 "valor" => $graulatitude."º ".$minlatitude."' ".$seglatitude."'' ".$pololatitude)
			  );
	array_push($dadosPA, array(
							 "label" => "Longitude",
							 "valor" => $graulongitude."º ".$minlongitude."' ".$seglongitude."''")
			  );
/////////////////////////////////FIM////////////////////////////////////////



/////////////////////////////////PRONAC////////////////////////////////////////
if ( $atipronacquest == 't' )
{
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "PRONAC")
			  );
	array_push($dadosPA, array(
							 "label" => "Nº do PRONAC",
							 "valor" => $atipronac)
			  );
	array_push($dadosPA, array(
							 "label" => "Nome do Proponente",
							 "valor" => $atiproponente)
			  );
	array_push($dadosPA, array(
							 "label" => "Tipo",
							 "valor" => $tiposetorcultural)
			  );
	array_push($dadosPA, array(
							 "label" => "Forma de Seleção",
							 "valor" => $tiposetorcultural)
			  );
	array_push($dadosPA, array(
							 "label" => "Nome do Representante",
							 "valor" => $atiemailrepresentante)
			  );
	array_push($dadosPA, array(
							 "label" => "E-mail do Representante",
							 "valor" => $atirepresentante)
			  );
}
else
{
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "PRONAC - NÃO")
			  );
}
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////EMENDA////////////////////////////////////////
if ( $atiemenda == 't' )
{
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "Emenda")
			  );
	array_push($dadosPA, array(
							 "label" => "Tipo Autor",
							 "valor" => $tipoautor)
			  );
	array_push($dadosPA, array(
							 "label" => "Partido",
							 "valor" => $partido)
			  );
	array_push($dadosPA, array(
							 "label" => "Autor",
							 "valor" => $autor)
			  );
}
else
{
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "Emenda - NÃO")
			  );
}
/////////////////////////////////FIM////////////////////////////////////////


/////////////////////////////////ORÇAMENTO E CONTRATAÇÃO////////////////////////////////////////
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "Orçamento e Contratação")
			  );
	array_push($dadosPA, array(
							 "label" => "Situação do processo de contratação",
							 "valor" => $situacaocontratacao)
			  );
	array_push($dadosPA, array(
							 "label" => "Forma de Contratação",
							 "valor" => $instrumentocontratacao)
			  );
	array_push($dadosPA, array(
							 "label" => "Número",
							 "valor" => $atinumeroinstrumento)
			  );
	array_push($dadosPA, array(
							 "label" => "Data de Início",
							 "valor" => $atidatainicio)
			  );
	array_push($dadosPA, array(
							 "label" => "Data de Término",
							 "valor" => $atidatafim)
			  );
	array_push($dadosPA, array(
							 "label" => "Data da Aprovação da Prestação de Contas",
							 "valor" => $atidataprestacao)
			  );
/////////////////////////////////FIM////////////////////////////////////////



/////////////////////////////////VALOR ESTIMADO////////////////////////////////////////
	array_push($dadosPA, array(
							 "label" => "",
							 "valor" => "Valor Estimado")
			  );
	array_push($dadosPA, array(
							 "label" => "Custeio R$",
							 "valor" => number_format( $atiorcamentocusteio, 2, ",", "." ) )
			  );
	array_push($dadosPA, array(
							 "label" => "Capital R$",
							 "valor" => number_format( $atiorcamentocapital, 2, ",", ".") )
			  );
	array_push($dadosPA, array(
							 "label" => "Total R$",
							 "valor" => number_format( $atiorcamento, 2, ",", "." ) )
			  );

	$cabecalho = array( 'Código', 'Valor');
	$arOrcaFontes = $db->carregar("SELECT fon.fondsc, ocf.opfvalor FROM pde.orcamentopafonte ocf JOIN sisplan.fonterecurso fon ON fon.fonid = ocf.fonid  WHERE ocf.atiid = ".$atiid);
	$arOrcaFontes = $arOrcaFontes ? $arOrcaFontes : array();

	$out = '
	<table width="100%" bgcolor="#ffffff" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
		<thead>
			<tr>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Código</td>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor</td>
			</tr>
		</thead>
		<tbody>';
	foreach ( $arOrcaFontes as $campo )
	{
		$out .= '
			<tr bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';">
				<td align="right" style="color:#999999;" title="Código">'.$campo['fondsc'].'</td>
				<td align="right" style="color:#999999;" title="Valor">'.number_format( $campo['opfvalor'], 2, ",", "." ).'<br></td>
			</tr>';
		$total += $campo['opfvalor'];
	}
	$out .= '
		</tbody>
		<tfoot>
			<tr>
				<td align="right" title="Código">Totais:</td>
				<td align="right" title="Valor">'.number_format( $total, 2, ",", "." ).'</td>
			</tr>
		</tfoot>
	</table>';
	array_push($dadosPA, array(
							 "label" => "Orçamento por Fontes R$",
							 "valor" => $out )
			  );
/////////////////////////////////FIM////////////////////////////////////////


	$html = "<table align='center' width='95%' border='0' cellpadding='2' cellspacing='1'>";

	$html .= "<tr><td bgcolor='#CCCCCC' colspan='2'><center><b>{$txt}</b></center></td></tr>";

	$html .= "<tr><td colspan='2' bgcolor='#CCCCCC'><center><b>Projeto</b></center></td></tr>";

	if ( $mostraDados)
	{
		foreach($dadosPA as $dado){

			if ($dado['label']){
				$td = "<td bgcolor='#CCCCCC' width='30%' align='right'><b>{$dado['label']}:</b></td>
					   <td bgcolor='#DFDFDF'>" . ($dado['valor'] ? $dado['valor'] : '&nbsp;') . "</td>";
			}else{
				$td = "<td colspan='2' bgcolor='#CCCCCC'><center><b>{$dado['valor']}</b></center></td>";
			}

			$html .= "<tr>{$td}</tr>";
		}
	}

	$html .= "</table>";

	return $html;
}

// ATIVIDADE ///////////////////////////////////////////////////////////////////


function atividade_inserir( $atividade, $titulo ){
	global $db;
	$sql = sprintf(
		"insert into pde.atividade (
			atiidpai, atidescricao, atiordem, _atiprojeto, acaid
		) values (
			%d,
			'%s',
			( select coalesce( max(atiordem), 0 ) + 1 from pde.atividade where atistatus = 'A' and atiidpai = %d ),
			( select _atiprojeto from pde.atividade where atiid = %d ),
			( select acaid from pde.atividade where atiid = %d )
		)",
		$atividade,
		$titulo,
		$atividade,
		$atividade,
		$atividade # adaptação necessária para que o módulo de monitoramento funcione
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_listar( $atividade, $profundidade = 0, $situacao = array(), $usuario = null, $perfil = array() ){
	global $db;

	// captura as opções
	$atividade    = (integer) $atividade;
	$profundidade = (integer) $profundidade;
	$situacao     = (array) $situacao;
	$usuario      = (string) $usuario;

	// identifica a atividade e o projeto
	$atividade = $atividade ? $atividade : PROJETO;
	$projeto   = (integer) $db->pegaUm( "select _atiprojeto from pde.atividade where atiid = $atividade" );
	if ( $projeto != PROJETO ) {
		$atividade = (integer) PROJETO;
		$projeto   = (integer) PROJETO;
	}

	// identifica o nó de origem
	$sql_filhas = "";
	if ( $atividade ) {
		$numero = $db->pegaUm( "select _atinumero from pde.atividade where atiid = $atividade" );
		if ( $numero ) {
			//$sql_filhas = " and ( a._atinumero like '$numero.%' ) ";
			$sql_filhas = " and ( substr( a._atinumero, 0, " . ( strlen( $numero ) + 2 ) . " ) = '" . $numero .  ".' ) ";
		}
	}

	// restringe a profundidade
	$sql_profundidade = "";
	if ( $profundidade > 0 ) {
		$sql_profundidade = " and ( a._atiprofundidade <= $profundidade ) ";
	}

	// restringe as situações
	$sql_situacao = "";
	if ( count( $situacao ) > 0 ) {
		$sql_situacao = " and a.esaid in (". implode( ',', $situacao ) .") ";
		$sql_situacao_restricao = " and a.esaid not in (". implode( ',', $situacao ) .") ";
	}

	// restringe por responsabilidade
	$sql_responsabilidade = "";
	if ( $usuario ) {
		$sql_perfil = "";
		if ( !empty( $perfil ) ) {
			$sql_perfil = " and ur.pflcod in ( ". implode( ",", $perfil ) ." ) ";
		}
		$sql = sprintf(
			"select a._atinumero
			from seguranca.usuario u
			inner join pde.usuarioresponsabilidade ur on ur.usucpf = u.usucpf %s
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join pde.atividade a on a.atiid = ur.atiid
			where
				u.suscod = 'A'
				and u.usucpf = '%s'
				and ur.rpustatus = 'A'
				and a._atiprojeto = %d
				and a.atiid != a._atiprojeto
				and a.atistatus = 'A'
				%s %s %s",
			$sql_perfil,
			$usuario,
			$projeto,
			$sql_filhas,
			$sql_profundidade,
			$sql_situacao
		);
		$numeros = array();
		foreach( (array) $db->carregar( $sql ) as $responsabilidade ) {
			$rastro = array();
			foreach( explode( ".", $responsabilidade['_atinumero'] ) as $item ){
				array_push( $rastro, sprintf( "%04d", $item ) );
			}
			$numero = implode( $rastro );
			//array_push( $numeros, " a._atiordem like '" . $numero ."%' " );
			array_push( $numeros, " substr( a._atinumero, 0, " . ( strlen( $numero ) + 1 ) . " ) = '" . $numero . "' " );
			foreach ( $rastro as $chave => $ordem ) {
				if ( $chave == 0 ) continue;
				$numero = implode( "", array_slice( $rastro, 0, $chave ) );
				array_push( $numeros, " a._atiordem = '" . $numero ."' " );
			}
		}
		$numeros = array_unique( $numeros );
		$sql_responsabilidade = " and ( ". implode( ' or ', $numeros ) ." ) ";
	}

	$sql_situacao_restricao = false;
	$sql_restricao = "";
	if ( $sql_situacao_restricao ) {
		$sql = sprintf(
			"select a._atinumero
			from pde.atividade a
			where
				a._atiprojeto = %d
				and a.atiid != a._atiprojeto
				and a.atistatus = 'A'
				%s %s %s",
			$projeto,
			$sql_filhas,
			$sql_profundidade,
			$sql_situacao_restricao
		);
		$restricao = array();
		$atinumeros = array();
		foreach( (array) $db->carregar( $sql ) as $atividade ) {
			if ( !$atividade['_atinumero'] ) {
				break;
			}
			array_push( $atinumeros, $atividade['_atinumero'] );
		}
		$numerosFinais = array();
		foreach ( array_unique( $atinumeros ) as $atinumero ) {
			$tamanho = strlen($atinumero );
			if ( !array_key_exists( $tamanho, $numerosFinais ) )
			{
				$numerosFinais[$tamanho] = array();
			}
			array_push( $numerosFinais[$tamanho], $atinumero . "." );
		}
		foreach ( $numerosFinais as $tamanho => $valores )
		{
			array_push( $restricao, " substr( a._atinumero, 0, " . ( $tamanho + 2 ) . " ) not in ( '" . implode( "','", $valores ) . "' ) " );
		}
		if ( count( $restricao ) > 0 ) {
			$sql_restricao = " and ( ". implode( ' and ', $restricao ) . " ) ";
		}
	}


	$sql = sprintf(
		"select
			a.atiid,
			a.aticodigo,
			a.atidescricao,
			--a.atidetalhamento,
			--a.atimeta,
			--a.atiinterface,
			a.atidatainicio,
			a.atidatafim,
			--a.atisndatafixa,
			a.atistatus,
			a.atiordem,
			--a.atinumeracao,
			--a.atiidpredecessora,
			a.atiidpai,
			--a.usucpf,
			--a.tatcod,
			a.esaid,
			a.atidataconclusao,
			a.atiporcentoexec,
			a._atiprojeto,
			--a._atiordem,
			a._atinumero,
			a._atiprofundidade,
			a._atiirmaos,
			a._atifilhos,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			--u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			coalesce( restricoes, 0 ) as qtdrestricoes,
			coalesce( anexos, 0 ) as qtdanexos,
			a._atiprofundidade as profundidade,
			a._atinumero as numero,
			a._atifilhos as filhos
		from pde.atividade a
		inner join pde.estadoatividade ea on
			ea.esaid = a.esaid
		left join pde.usuarioresponsabilidade ur on
			ur.atiid = a.atiid and
			ur.rpustatus = 'A' and
			ur.pflcod = %d
		left join seguranca.perfilusuario pu on
			pu.pflcod = ur.pflcod and
			pu.usucpf = ur.usucpf
		left join seguranca.usuario u on
			u.usucpf = pu.usucpf and
			u.suscod = 'A'
		left join public.unidade uni on
			uni.unicod = u.unicod and
			uni.unitpocod = 'U' and
			uni.unistatus = 'A'
		left join public.unidadegestora ug on
			ug.ungcod = u.ungcod and
			ug.ungstatus = 'A'
		left join (
			select atiid, count(*) as restricoes
			from pde.observacaoatividade
			where obsstatus = 'A' and obssolucao = false
			group by atiid ) restricao on
				restricao.atiid = a.atiid
		left join (
			select atiid, count(*) as anexos
			from pde.anexoatividade
			where anestatus = 'A'
			group by atiid ) anexo on
				anexo.atiid = a.atiid
		where
			a._atiprojeto = %d
			and a.atiid != a._atiprojeto
			and a.atistatus = 'A'
			%s %s %s %s %s
		order by _atiordem",
		PERFIL_GERENTE,
		$projeto,
		$sql_filhas,
		$sql_profundidade,
		$sql_situacao,
		$sql_restricao,
		$sql_responsabilidade
	);
	//dbg( $sql, 1 );
	return $db->carregar( $sql );
}

function atividade_excluir( $atiid ){
	global $db;
	// captura as informações da atividade a ser excluída
	$sql = sprintf( "select * from pde.atividade a where a.atiid = %s and a.atistatus = 'A'", $atiid );
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// exclui a atividade
	$sql = sprintf( "update pde.atividade set atistatus = 'I' where atiid = %s", $atividade['atiid'] );
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	// reordena as atividades que tem o mesmo pai
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem - 1 where atiidpai = %s and atiordem > %s and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	return true;
}

function atividade_pegar( $atividade ){
	global $db;
	$sql = sprintf(
		"select a.*, e.esadescricao, sub.numero, sub.projeto
		from pde.atividade a
		left join pde.estadoatividade e on e.esaid = a.esaid
		inner join pde.f_dadosatividade( %d ) as sub on sub.atiid = a.atiid
		where a.atiid = %d and atistatus = 'A'",
		(integer) $atividade,
		(integer) $atividade
	);

	$registro = $db->pegaLinha( $sql );
	if ( is_array( $registro ) ) {
		return $registro;
	}
	return null;
}

function atividade_pegar_projeto( $atividade ){
	global $db;
	$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $atividade );
	return $db->pegaUm( $sql );
}

/**
 * Retorna as atividades que estão acima da atividade indicada exceto o projeto,
 * que é a atividade raiz.
 *
 * @return array
 */
function atividade_pegar_rastro( $numero ){
	global $db;
	$numero_original = $numero;
	$condicao = array();
	array_push( $condicao, " a._atinumero = '$numero' " );
	while( ( $posicao = strrpos( $numero, '.' ) ) !== false ) {
		$numero = substr( $numero, 0, $posicao );
		array_push( $condicao, " a._atinumero = '$numero' " );
	}
	if ( count( $condicao ) == 0 ) {
		return array();
	}
	$sql = sprintf(
		"select
			a._atinumero as numero,
			a._atiprofundidade as profundidade,
			a._atiirmaos as irmaos,
			a._atifilhos as filhos,
			a.atidescricao,
			a.atiid,
			a.atiidpai,
			a.atidatainicio,
			a.atidatafim,
			a.atiordem,
			a.atiporcentoexec,
			a.esaid,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			uni.unidsc,
			ug.ungdsc
		from pde.atividade a
			left join pde.estadoatividade ea on
				ea.esaid = a.esaid
			left join pde.usuarioresponsabilidade ur on
				ur.atiid = a.atiid and ur.rpustatus = 'A' and ur.pflcod = %d
			left join seguranca.usuario u on
				u.usucpf = ur.usucpf and u.usustatus = 'A'
			left join public.unidade uni on
				uni.unicod = u.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			left join public.unidadegestora ug on
				ug.ungcod = u.ungcod and
				ug.ungstatus = 'A'
		where
			a._atiprojeto = %d and
			a.atiidpai is not null and
			a.atistatus = 'A' and
			( %s )
		order by a._atiordem",
		PERFIL_GERENTE,
		PROJETO,
		implode( ' or ', $condicao )
	);
	$rastro = $db->carregar( $sql );
	return $rastro && count( $rastro ) == substr_count( $numero_original, "." ) + 1 ? $rastro : array();
}

function atividade_pegar_filhas( $projeto, $atividade = null, $usuario = null, $profundidade = null ){
	global $db;
	$profundidade = (string) $profundidade;
	if ( $profundidade != '' ) {
		$profundidade = (integer) $profundidade;
		if ( $atividade ) {
			$sql = "select profundidade from pde.f_dadosatividade( " . $atividade . " )";
			$profundidade = $db->pegaUm( $sql ) + $profundidade;
		} else {
			$profundidade++;
		}
		$condicao_profundidade = " and la.profundidade <= " . $profundidade;
	} else {
		$condicao_profundidade = "";
		$profundidade = null;
	}
	if ( $usuario ) {
		return atividade_pegar_sob_responsabilidade( $projeto, $usuario, $profundidade );
	}
	if ( $atividade ) {
		$sql = sprintf(
			"select
				la.numero,
				la.profundidade,
				la.irmaos,
				la.filhos,
				a.atidescricao,
				a.atiid,
				a.atiidpai,
				a.atidatainicio,
				a.atidatafim,
				a.atidataconclusao,
				a.atiordem,
				a.atiporcentoexec,
				a.esaid,
				ea.esadescricao,
				u.usunome,
				u.usunomeguerra,
				u.usucpf,
				u.usuemail,
				u.usufoneddd,
				u.usufonenum,
				uni.unidsc,
				ug.ungdsc,
				coalesce(qtdrestricoes,0) as qtdrestricoes,
				coalesce(qtdanexos,0) as qtdanexos
			from pde.f_dadosatividade( %d ) as da
				inner join pde.f_dadostodasatividades() as la on
					la.numero like da.numero || '.%%' or
					la.numero = da.numero
				inner join pde.atividade a on
					a.atiid = la.atiid
				left join pde.estadoatividade ea on
					ea.esaid = a.esaid
				left join pde.usuarioresponsabilidade ur on
					ur.atiid = la.atiid and
					ur.rpustatus = 'A' and
					ur.pflcod = %d
				left join seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu.usucpf and
					u.suscod = 'A'
				left join public.unidade uni on
					uni.unicod = u.unicod and
					uni.unitpocod = 'U' and
					uni.unistatus = 'A'
				left join public.unidadegestora ug on
					ug.ungcod = u.ungcod and
					ug.ungstatus = 'A'
				left join (
					select atiid, count(*) as qtdrestricoes
					from pde.observacaoatividade
					where obsstatus = 'A' and obssolucao = false
					group by atiid ) restricao on restricao.atiid = a.atiid
				left join (
					select atiid, count(*) as qtdanexos
					from pde.anexoatividade
					where anestatus = 'A'
					group by atiid ) anexo on anexo.atiid = a.atiid
			where
				la.projeto = %d and
				la.projeto != la.atiid and
				a.atistatus = 'A'
				%s
			order
				by la.ordem",
			$atividade,
			PERFIL_GERENTE,
			$projeto,
			$condicao_profundidade
		);
	} else {
		$sql = sprintf(
			"
			select
				la.numero,
				la.profundidade,
				la.irmaos,
				la.filhos,
				a.atidescricao,
				a.atiid,
				a.atiidpai,
				a.atidatainicio,
				a.atidatafim,
				a.atidataconclusao,
				a.atiordem,
				a.atiporcentoexec,
				a.esaid,
				ea.esadescricao,
				u.usunome,
				u.usunomeguerra,
				u.usucpf,
				u.usuemail,
				u.usufoneddd,
				u.usufonenum,
				uni.unidsc,
				ug.ungdsc,
				coalesce(qtdrestricoes,0) as qtdrestricoes,
				coalesce(qtdanexos,0) as qtdanexos
			from pde.f_dadostodasatividades() la
				inner join pde.atividade a on
					a.atiid = la.atiid
				left join pde.estadoatividade ea on
					ea.esaid = a.esaid
				left join pde.usuarioresponsabilidade ur on
					ur.atiid = la.atiid and
					ur.rpustatus = 'A' and
					ur.pflcod = %d
				left join seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu.usucpf and
					u.suscod = 'A'
				left join public.unidade uni on
					uni.unicod = u.unicod and
					uni.unitpocod = 'U' and
					uni.unistatus = 'A'
				left join public.unidadegestora ug on
					ug.ungcod = u.ungcod and
					ug.ungstatus = 'A'
				left join (
					select atiid, count(*) as qtdrestricoes
					from pde.observacaoatividade
					where obsstatus = 'A' and obssolucao = false
					group by atiid ) restricao on restricao.atiid = a.atiid
				left join (
					select atiid, count(*) as qtdanexos
					from pde.anexoatividade
					where anestatus = 'A'
					group by atiid ) anexo on anexo.atiid = a.atiid
			where
				la.projeto = %d and
				la.projeto != la.atiid and
				a.atistatus = 'A'
				%s
			order by
				la.ordem",
			PERFIL_GERENTE,
			$projeto,
			$condicao_profundidade
		);
	}
	$lista = $db->carregar( $sql );
	if ( is_array( $lista ) ) {
		return $lista;
	}
	return array();
}

function atividade_pegar_sob_responsabilidade( $projeto, $usuario, $profundidade = null ){
	global $db;
	if ( $profundidade !== null ) {
		$condicao_profundidade = " and folha.profundidade <= " . $profundidade;
	} else {
		$condicao_profundidade = "";
	}
	$sql = sprintf(
		"select
			folha.numero,
			folha.profundidade,
			folha.irmaos,
			folha.filhos,
			a.atidescricao,
			a.atiid,
			a.atiidpai,
			a.atidatainicio,
			a.atidatafim,
			a.atidataconclusao,
			a.atiordem,
			a.atiporcentoexec,
			a.esaid,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			coalesce(qtdrestricoes,0) as qtdrestricoes,
			coalesce(qtdanexos,0) as qtdanexos
		from pde.usuarioresponsabilidade ur
			inner join pde.f_dadostodasatividades() as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join pde.f_dadostodasatividades() as folha on
				folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			inner join pde.atividade a on
				a.atiid = folha.atiid
			left join pde.estadoatividade ea on
				ea.esaid = a.esaid
			left join pde.usuarioresponsabilidade ur2 on
				ur2.atiid = folha.atiid and ur2.rpustatus = 'A' and ur2.pflcod = %d
			left join seguranca.perfilusuario pu2 on pu2.pflcod = ur2.pflcod and pu2.usucpf = ur2.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu2.usucpf and
					u.suscod = 'A'
			left join public.unidade uni on
				uni.unicod = u.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			left join public.unidadegestora ug on
				ug.ungcod = u.ungcod and
				ug.ungstatus = 'A'
			left join (
				select atiid, count(*) as qtdrestricoes
				from pde.observacaoatividade
				where obsstatus = 'A' and obssolucao = false
				group by atiid ) restricao on restricao.atiid = a.atiid
			left join (
				select atiid, count(*) as qtdanexos
				from pde.anexoatividade
				where anestatus = 'A'
				group by atiid ) anexo on anexo.atiid = a.atiid
		where
			ur.rpustatus = 'A' and
			ur.usucpf = '%s' and
			folha.projeto = %d and
			folha.projeto != folha.atiid and
			raiz.projeto = %d
			%s
		order by folha.ordem",
		PERFIL_GERENTE,
		$usuario,
		$projeto,
		$projeto,
		$condicao_profundidade
	);
	$lista = $db->carregar( $sql );
	if ( !is_array( $lista ) ) {
		return array();
	}
	$lista_final = array();
	foreach ( $lista as $item ) {
		if ( array_key_exists( $item['numero'], $lista_final ) ) {
			continue;
		}
		// adiciona pais (caso o pai não esteja na lista)
		$numero_pai = substr( $item['numero'], 0, strrpos( $item['numero'], '.' ) );
		if ( $numero_pai && !array_key_exists( $numero_pai, $lista_final ) ) {
			$rastro_pai = atividade_pegar_rastro( $item['numero'] );
			foreach ( $rastro_pai as $item_pai ) {
				if ( !array_key_exists( $item_pai['numero'], $lista_final ) ) {
					$lista_final[$item_pai['numero']] = $item_pai;
				}
			}
		}
		// adiciona item à lista
		$lista_final[$item['numero']] = $item;
	}
	return array_values( $lista_final );
}

function atividade_calcular_dados( $atividade ){
	global $db;
	// pega dados da atividade
	$atividade = (integer) $atividade;
	$sql = "select _atiordem, _atinumero, _atiprofundidade, _atiprojeto from pde.atividade where atiid = " . $atividade;
	$pai = $db->recuperar( $sql );

	// pega filhos
	$sql = "select atiid, atiordem from pde.atividade where atiidpai = " . $atividade . " and atistatus = 'A'";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	$sql = "update pde.atividade set _atifilhos = " . count( $filhos ) . " where atiid = " . $atividade;
	$db->executar( $sql, false );

	// atualiza filhos
	foreach ( $filhos as $filho ){
		$_atinumero  = ( $pai['_atinumero'] ? $pai['_atinumero'] . "." : '' ) . $filho['atiordem'];
		$_atiordem   = ( $pai['_atiordem'] ? $pai['_atiordem'] : '' ) . sprintf( '%04d', $filho['atiordem'] );
		$_atiprojeto = (integer) $pai['_atiprojeto'];
		$sql = "
			update pde.atividade
			set
				_atinumero = '" . $_atinumero . "',
				_atiordem = '" . $_atiordem . "',
				_atiprofundidade = " . ( $pai['_atiprofundidade'] + 1 ) . ",
				_atiirmaos = " . count( $filhos ) . ",
				_atiprojeto = " . $_atiprojeto . "
			where atiid = " . $filho['atiid'];
		$db->executar( $sql, false );
		atividade_calcular_dados( $filho['atiid'] );
	}
}

function atividade_calcular_possibilidade_mudar_data( $intIdAtividade , $strNovaDataInicio = null, $strNovaDataFim = null, $strNovaDataConclusao = null ){
	global $db;

	$sql = sprintf(
		"SELECT atidatainicio , atidatafim, atidataconclusao, esaid from pde.atividade where atiid = %d",
		$intIdAtividade
	);

	$arrAtiDatas = $db->recuperar( $sql );

	if( $strNovaDataInicio !== null )
	{
		$arrAtiDatas[ 'atidatainicio'  ] = formata_data_sql( $strNovaDataInicio );
	}
	if( $strNovaDataFim !== null )
	{
		$arrAtiDatas[ 'atidatafim'  ] = formata_data_sql( $strNovaDataFim );
	}
	if( $strNovaDataConclusao !== null )
	{
		$arrAtiDatas[ 'atidataconclusao'  ] = formata_data_sql( $strNovaDataConclusao );
	}

	$intDataInicio	= strtotime( $arrAtiDatas[ 'atidatainicio' ] );

	if( (integer) $arrAtiDatas['esaid'] == (integer) STATUS_CONCLUIDO )
	{
		if( $arrAtiDatas[ 'atidataconclusao' ] != null )
		{
			$intDataTermino = strtotime(  $arrAtiDatas[ 'atidataconclusao' ] );
		}
		else
		{
			$intDataTermino = null;
		}
	}
	else
	{
		if(  $arrAtiDatas[ 'atidatafim' ] != null )
		{
			$intDataTermino = strtotime( $arrAtiDatas[ 'atidatafim' ] );
		}
		else
		{
			$intDataTermino = null;
		}
	}
	if	(
			( $intDataInicio !== null )
			&&
			( $intDataTermino !== null )
			&&
			( $intDataInicio > $intDataTermino )
		)
	{
		return false;
	}
	return true;
}


// RESPONSABILIDADE ////////////////////////////////////////////////////////////


/**
 * Atribui responsabilidade aos usuários na atividade indicada segundo o perfil
 * especificado.
 *
 * @return boolean
 */
function atividade_atribuir_responsavel( $atividade, $perfil, $usuarios ){
	global $db;
	$sql = sprintf(
		"update pde.usuarioresponsabilidade
		set rpustatus = 'I'
		where pflcod = %d and atiid  = %d",
		$perfil,
		$atividade
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	foreach ( $usuarios as $usuario ) {
		if ( empty( $usuario ) ) {
			continue;
		}
		$sql = "select count(*) from seguranca.perfilusuario where pflcod = $perfil and usucpf = '$usuario'";
		$possui_perfil = $db->pegaUm( $sql );
		if ( !$possui_perfil )
		{
			$sql = "insert into seguranca.perfilusuario ( pflcod, usucpf ) values ( $perfil, '$usuario' )";
			$db->executar( $sql );
		}
		$sql = sprintf(
			"select count(*) from pde.usuarioresponsabilidade
			where usucpf = '%s' and pflcod = %d and atiid = %d",
			$usuario,
			$perfil,
			$atividade
		);
		if( (boolean) $db->pegaUm( $sql ) ) {
			$sql = sprintf(
				"update pde.usuarioresponsabilidade
				set rpustatus = 'A'
				where usucpf = '%s' and pflcod = %d and atiid = %d",
				$usuario,
				$perfil,
				$atividade
			);
		} else {
			$sql = sprintf(
				"insert into pde.usuarioresponsabilidade (
					usucpf, pflcod, atiid, rpustatus
				) values (
					'%s', %d, %d, 'A'
				)",
				$usuario,
				$perfil,
				$atividade
			);
		}
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			return false;
		}
		$db->alterar_status_usuario( $usuario, 'A', 'Atribuição de responsabilidade em atividade ou projeto.', $_SESSION['sisid'] );
	}
	return true;
}


/**
 * @return boolean
 */
function unidadeexecutora_verificar_responsabilidade( $uexid, $usuario = null ){
	global $db;
	$usuario = (!empty($usuario) ? $usuario : $_SESSION['usucpf']);

	if (!$uexid || !$usuario){
		return false;
	}elseif ( $db->testa_superuser() ) {
		return true;
	}

	$sql = "SELECT
				COUNT(*)
			FROM
				planointerno.unidadeexecutora u
			JOIN
				sisplan.usuarioresponsabilidade ur on ur.uexid = u.uexid
			WHERE
				u.uexstatus = 'A'
				AND u.uexid = {$uexid}
				AND ur.usucpf = '" . $usuario . "'";

	return (boolean) $db->pegaUm($sql);
}

/**
 * @return boolean
 */
function atividade_verificar_responsabilidade( $atividade, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	if ( $_SESSION["sisid"] == 1 ) {
		return acao_verificar_responsabilidade( $atividade, $usuario );
	}
	if ( !array_key_exists( $usuario, $permissoes ) ) {
		$sql = sprintf(
			"select folha.atiid
			from pde.usuarioresponsabilidade ur
				inner join pde.f_dadostodasatividades() as raiz on
					raiz.atiid = ur.atiid
				inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
				inner join pde.f_dadostodasatividades() as folha on
					folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			where ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
			group by folha.atiid",
			$usuario,
			PERFIL_GERENTE,
			PERFIL_EQUIPE_APOIO_GERENTE
		);
		$lista = $db->carregar( $sql );
		$permissoes[$usuario] = array();
		if ( is_array( $lista ) ) {
			foreach ( $lista as $item ) {
				array_push( $permissoes[$usuario], $item['atiid'] );
			}
		}
	}
	if ( !in_array( $atividade, $permissoes[$usuario] ) ) {
		$projeto = $db->pegaUm( "select _atiprojeto from pde.atividade where atiid = " . $atividade );
		if ( !$projeto ) {
			$projeto = $atividade;
		}
		return projeto_verificar_responsabilidade( $projeto );
	}
	return true;
}

/**
 * @return boolean
 */
function atividade_verificar_perfil( $atividade, $perfil, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas

	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];

	// CASO ESPECÍFICO PARA GERENTE DE PROJETO
	if ( $perfil == PERFIL_GERENTE )
	{
		$sql = "select usucpf from pde.atividade where atiid = " . $atividade;
		return $db->pegaUm( $sql ) == $usuario;
	}

	if ( $db->testa_superuser() ) {
		return true;
	}
	if ( !array_key_exists( $perfil, $permissoes ) ) {
		$sql = sprintf(
			"select folha.atiid
			from pde.usuarioresponsabilidade ur
				inner join pde.f_dadostodasatividades() as raiz on
					raiz.atiid = ur.atiid
				inner join pde.f_dadostodasatividades() as folha on
					folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			where ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod = %d
			group by folha.atiid",
			$usuario,
			$perfil
		);
		$lista = $db->carregar( $sql );
		$permissoes[$usuario] = array();
		if ( is_array( $lista ) ) {
			foreach ( $lista as $item ) {
				array_push( $permissoes[$usuario], $item['atiid'] );
			}
		}
	}
	return in_array( $atividade, $permissoes[$usuario] );
}

function projeto_verificar_responsabilidade( $projeto, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	if ( $_SESSION["sisid"] == 1 ) {
		return acao_verificar_responsabilidade( $projeto, $usuario );
	}
	$sql = sprintf(
		"select count(*)
		from pde.usuarioresponsabilidade ur
			inner join pde.f_dadostodasatividades() as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join pde.f_dadostodasatividades() as folha on
				folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
		where ur.atiid = %d and ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
		",
		$projeto,
		$usuario,
		PERFIL_GESTOR,
		PERFIL_EQUIPE_APOIO_GESTOR
	);
//	dbg($sql, 1);
	return $db->pegaUm( $sql ) > 0;
}

/**
 * @return boolean
 */
function usuario_possui_perfil( $perfil, $usuario = null ){
	global $db;
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	$sql = sprintf(
		"select count( * )
		from seguranca.perfilusuario
		where
			usucpf = '%s' and
			pflcod = %d",
		$usuario,
		$perfil
	);
	return (boolean) $db->pegaUm( $sql );
}

/**
 * @return boolean
 */
function atividade_obra( $atiid ){
	global $db;

	$sql = sprintf(
		"select count(1)
		from pde.atividade
		where
			atiobra = true and atiid = %d",
		$atiid
	);

	return (boolean) $db->pegaUm( $sql );
}

// ORDEM E NÍVEL DAS ATIVIDADES //////////////////////////////////////////////////////


function atividade_ordem_subir( $atiid ){
	global $db;
	// verifica se está no topo
	$sql = sprintf(
		"select a.* from pde.atividade a where a.atiid = %d and a.atistatus = 'A' and a.atiordem > 1",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return true;
	}
	// altera a posição dos irmãos
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
		$atividade['atiordem'],
		$atividade['atiordem'] - 1,
		$atividade['atiidpai']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// altera a posição da atividade
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
		$atividade['atiordem'] - 1,
		$atividade['atiid']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_ordem_descer( $atiid ){
	global $db;
	// verifica se está no final
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A' and a1.atiordem < ( select count(*) from pde.atividade a2 where a2.atiidpai = a1.atiidpai and atistatus = 'A' )",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return true;
	}
	// altera a posição dos irmãos
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
		$atividade['atiordem'],
		$atividade['atiordem'] + 1,
		$atividade['atiidpai']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	$sql = sprintf(
		"update pde.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
		$atividade['atiordem'] + 1,
		$atividade['atiid']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_profundidade_esquerda( $atiid ){
	global $db;
	// carrega os dados da atividade
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// carrega os dados do antigo pai da atividade
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
		$atividade['atiidpai']
	);
	$atividade_pai = $db->pegaLinha( $sql );
	if ( !$atividade_pai ) {
		$db->rollback();
		return false;
	}
	// desloca os novos irmãos para baixo
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem + 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
		$atividade_pai['atiidpai'],
		$atividade_pai['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// desloca os antigos irmãos para cima
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem - 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// troca o pai (pelo avô)
	$sql = sprintf(
		"update pde.atividade set atiidpai = %d, atiordem = %d where atistatus = 'A' and atiid = %d",
		$atividade_pai['atiidpai'],
		$atividade_pai['atiordem'] + 1,
		$atividade['atiid']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_profundidade_direita( $atiid ){
	global $db;
	// carrega os dados da atividade
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// carrega o novo pai (irmão que está uma posição acima)
	$sql = sprintf(
		"select a1.* from pde.atividade a1 where atiidpai = %d and atiordem = %d and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem'] - 1
	);
	$atividade_pai = $db->pegaLinha( $sql );
	if ( !$atividade_pai ) {
		$db->rollback();
		return false;
	}
	// desloca os antigos irmãos para cima
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem - 1 where atiidpai = %d and atiordem > %d and atistatus = 'A' ",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// troca o pai (pelo antigo irmão)
	$sql = sprintf(
		"update pde.atividade set atiidpai = %d, atiordem = 1 + ( select count(*) from pde.atividade where atiidpai = %d and atistatus = 'A' ) where atiid = %d and atistatus = 'A'",
		$atividade_pai['atiid'],
		$atividade_pai['atiid'],
		$atividade['atiid']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}


// ÁRVORE //////////////////////////////////////////////////////////////////////


function arvore_ocultar_item( $atividade ){
	if ( !isset( $_SESSION['arvore'] ) ) {
		$_SESSION['arvore'] = array();
	}
	$_SESSION['arvore'][$atividade] = $atividade;
}

function arvore_exibir_item( $atividade ){
	if ( !isset( $_SESSION['arvore'] ) ) {
		$_SESSION['arvore'] = array();
	}
	unset( $_SESSION['arvore'][$atividade] );
}

function arvore_verificar_exibicao_item( $numero, $ignorar = array() ){
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	$ignorar = $ignorar ? $ignorar : array();
	$arvore = arvore_pegar_estado_exibicao( $numero );
	// verifica o estado de exibição do item
	$numero = explode( '.', substr( $numero, 0, strrpos( $numero, '.' ) ) );
	for ( $i = count( $numero ); $i > 0; $i-- ) {
		$numero_atual = implode( '.', array_slice( $numero, 0, $i ) );
		if ( in_array( $numero_atual, $arvore ) && !in_array( $numero_atual, $ignorar ) ) {
			return false;
		}
	}
	return true;
}

function arvore_verificar_exibicao_filhos( $numero ){
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	$arvore = arvore_pegar_estado_exibicao( $numero );
	return in_array( $numero, $arvore );
}

function arvore_pegar_estado_exibicao( $numero ){
	static $arvore = null;
	global $db;
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	// verifica se há alguma informação na sessão
	if ( empty( $_SESSION['arvore'] ) ) {
		return array();
	}
	// carrega os números a partir dos ids gravados na sessão
	if ( !is_array( $arvore ) ) {
		$sql = sprintf(
			"select numero from pde.f_dadostodasatividades() where atiid in ( %s )",
			implode( ',', $_SESSION['arvore'] )
		);
		$arvore = array();
		$atividades = $db->carregar( $sql );
		if ( is_array( $atividades ) ) {
			foreach ( $atividades as $atividade ) {
				array_push( $arvore, $atividade['numero'] );
			}
		}
	}
	return $arvore;
}

function arvore_iniciar_dados_sessao(){
	global $db;
	$sql = "select atiid from pde.atividade where atistatus = 'A' and _atiprojeto = " . PROJETO;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	foreach ( $linhas as $linha ){
		arvore_ocultar_item( (integer) $linha['atiid'] );
	}
}


// OUTRAS FUNÇÕES //////////////////////////////////////////////////////////////


/**
 * Redireciona o navegador para a tela indicada.
 *
 * @return void
 */
function redirecionar( $modulo, $acao, $parametros = array() ) {
	$parametros = http_build_query( (array) $parametros, '', '&' );
	header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
	exit();
}

/**
 * Verifica se um projeto está selecionado.
 *
 * Caso uma atividade seja passada como parâmetro verifica se além de algum
 * projeto está selecionado essa atividade pertença ao projeto selecionado.
 * Essa função redireciona para a tela de projetos caso a verificação falhe.
 *
 * @param integer $atividade
 * @return void
 */
function projeto_verifica_selecionado( $atividade = null ) {
	global $db;
	$atividade = (integer) $atividade;
	// verifica se projeto está escolhido
	$sql = sprintf( "select count(atiid) from pde.atividade where atiid = %d and atistatus = 'A'", $_SESSION['projeto'] );
	if ( $db->pegaUm( $sql ) != 1 ) {
		redirecionar( $_SESSION['paginainicial'], 'A' );
	}
	// verifica se a atividade indicada pertence ao projeto atual
	if ( !$atividade ) {
		return;
	}
	$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $atividade );
	if ( $db->pegaUm( $sql ) != $_SESSION['projeto'] ) {
		redirecionar( $_SESSION['paginainicial'], 'A' );
	}
}


// OUTRAS FUNÇÕES //////////////////////////////////////////////////////////////


function registrar_mensagem( $mensagem ){
	if ( !isset( $_SESSION['mensagem'] ) ) {
		$_SESSION['mensagem'] = array();
	}
	array_push( $_SESSION['mensagem'], $mensagem );
}

function exibir_mensagens(){
	if ( !isset( $_SESSION['mensagem'] ) ) {
		$_SESSION['mensagem'] = array();
	}
	if ( count( $_SESSION['mensagem'] ) == 0 ) {
		return;
	}
	$htm = '<script language="javascript" type="text/javascript">';
	$htm .= 'alert("'. implode( "\n", $_SESSION['mensagem'] ) .'")';
	$htm .= '</script>';
	$_SESSION['mensagem'] = array();
	return $htm;
}

function acao_verificar_responsabilidade( $atividade, $usuario ){
	global $db;
	$ano = $_SESSION['exercicio'];
	$sql = <<<EOS
		select count( u.usucpf )
		from pde.atividade ati
		inner join monitora.acao aca on aca.acaid = ati.acaid
		inner join monitora.usuarioresponsabilidade ur on ur.acaid = aca.acaid
		inner join seguranca.perfil p on p.pflcod = ur.pflcod
		inner join seguranca.usuario u on u.usucpf = ur.usucpf
		inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
		where
		ati.atistatus = 'A' and ati.atiid = $atividade
		and aca.acastatus = 'A'
		and ur.rpustatus = 'A' and ur.prsano = '$ano'
		and p.pflstatus = 'A'
		and u.suscod = 'A' and u.usucpf = '$usuario'
		and us.suscod = 'A'
EOS;
	return $db->pegaUm( $sql ) > 0;
}

function toXmlMinc( $id, $arquivo )
{

	include APPRAIZ."includes/Snoopy.class.php";


	global $db;


	$s 				= new Snoopy;
	$s->agent 		= "";
	$s->_isproxy 	= false;
	$s->proxy_host	= "";
	$s->proxy_port	= "";
	$s->proxy_user	= "";
	$s->proxy_pass	= "";
	$s->results		= "";


	$strsql = 'SELECT * FROM pde.atividade WHERE atiid = '.$id;

	$nlinhas = count( $db->carregar($strsql) );

	$xml = '<?xml version="1.0" encoding="utf-8"?'.">\n";
	$xml.='<ArrayOf'.$arquivo.' xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://www.sigplan.gov.br/xml/">';

	$res = $db->carregar( $strsql );

	if( is_array( $res ) )
	{
		foreach( $res as $k =>$v)
		{
			$xml .= "\n  <" . $arquivo. ">\n";
			foreach( $v as $x =>$y)
			{
				${$x} = $y;

				if ( $y == 't' )
				{
					$y = '1';
				}
				elseif ( $y == 'f' )
				{
					$y = '0';
				}
				if ( !is_int( $x ) && $y != '' )
				{
					$y = utf8_encode( simec_htmlspecialchars( $y ) );
					$y = str_replace( "'", "&apos;", $y );
					$xml .= "    <" . $x . ">" . $y . "</" . $x . ">\n";
				}
			}
			$xml .= "  </" . $arquivo . ">";
		}
	}
	$xml .= "\n</ArrayOf" . $arquivo . ">";


	if( $_REQUEST['ususenha'] == "" )
	{
		$senha = $db->pegaUm( "select ususenha from seguranca.usuario where usucpf = '" . $_SESSION['usucpf'] . "'" );
		$_REQUEST['ususenha'] = md5_decrypt_senha( $senha, '' );
	}

	$header = sprintf(
		"%sservico.php?usucpf=%s&ususenha=%s&modulo=%s&sisid_pde=%s",
		URL_MINC,
		formatar_cpf($_SESSION['usucpf']),
		$_REQUEST['ususenha'],
		'sistema/comunica/servicos/receberpa',
		SISID_PDE
	);

	$arrayXML['xml']      = trim($xml);
	$arrayXML['atiid']    = $id;


	$_SESSION['senha_minc'] = $_REQUEST['ususenha'];

	$s->submit( $header, $arrayXML );

	// NOTE: debug com arquivo porque a requisicao vai acontecer em background
	// no workflow
	//
	// dbg( $arrayXML);
	// $fp = fopen( "/tmp/teste", "a" );
	// fputs( $fp, $s->results  );
	// fclose($fp);
	if ( $s->results )
		dbg($s->results,1);
	//
	// @TODO: fazer um tratamento se o minc recebeu e processou o xml e somente
	// ai retonar true
	return true;
	//dbg($s->results,1);

}


/**
 * Verifica se existe dados referente a situacao técnica de uma contratacao
 *
 * É obrigatório passar o parâmetro da tabela de contratacao para verificar
 * se existe dados na tabela.
 *
 * @param integer $cttid
 * @return void
 */
function verificasituacaocontratacao( $cttid, $tabela, $situacao = null ) {
	global $db;

	if ( !$cttid && !$tabela ) {
		return;
	}

	$cttid = (integer) $cttid;

	if ( $situacao ) $wh = " AND st".substr($tabela, strpos($tabela, 'o')+1, 1)."idsituacao = ".$situacao." ";

	// verifica se existe dados na tabela de instrucao tecnica
	$sql = sprintf( "select count(cttid) from sisplan.".$tabela." where st".substr($tabela, strpos($tabela, 'o')+1, 1)."status = 'A' and cttid = %d ".$wh." ", $cttid );
	if ( $db->pegaUm( $sql ) != 0 )
		{ return true; }
	else
		{ return false; }

}

/**
 * Retorna qual a situacao da tabela passada
 *
 * É obrigatório passar o parâmetro da tabela de contratacao para verificar
 * se existe dados na tabela.
 *
 * @param integer $cttid
 * @return void
 */
function retornasituacaocontratacao( $cttid, $tabela ) {
	global $db;

	if ( !$cttid && !$tabela ) {
		return;
	}

	$cttid = (integer) $cttid;

	// verifica se existe dados na tabela de instrucao tecnica
	$sql = sprintf( "select count(cttid) from sisplan.".$tabela." where cttid = %d ", $cttid );
	if ( $db->pegaUm( $sql ) != 0 )
		{ return true; }
	else
		{ return false; }

}

function atividade_pegar_contratacao( $atividade ){
	global $db;
	$sql = sprintf(
		"select c.*, st.sttdtprevista, st.sttdtconclusao, st.sttidsituacao, sa.stadtprevista, sa.stadtconclusao, sa.staidsituacao, sj.stjdtprevista, sj.stjdtconclusao, sj.stjidsituacao, sc.stcdtinicio, sc.stcdtprevisao, sc.stcdtconclusao, sc.stcnotaempenho, sc.stccontratado, sc.stcvalorcontratado, sc.tpcid, sc.stcidsituacao, sc.stcdtpublicacaoresult, sc.stcidlocalpubresult, sc.stcdtpublicacaoextrato, sc.stcidlocalpubextrato, se.stedtinicio, se.stedttermino, se.stenumparcelas, se.spcid, se.stedtprestacao, se.steidsituacao from pde.atividade a
		inner join sisplan.contratacao c on c.atiid = a.atiid
		left join sisplan.situacaotecnica st on st.cttid = c.cttid and sttstatus = 'A'
		left join sisplan.situacaoadmin sa on sa.cttid = c.cttid and stastatus = 'A'
		left join sisplan.situacaojuridica sj on sj.cttid = c.cttid and stjstatus = 'A'
		left join sisplan.situacaocontratacao sc on sc.cttid = c.cttid and stcstatus = 'A'
		left join sisplan.situacaoexecucao se on se.cttid = c.cttid and stestatus = 'A'
		where a.atiid = %d and cttstatus = 'A' and atistatus = 'A'",
		(integer) $atividade
	);

	$registro = $db->pegaLinha( $sql );
	if ( is_array( $registro ) ) {
		return $registro;
	}
	return null;
}

function atividade_pegar_aditivo( $sadid ){
	global $db;
	$sql = sprintf(
		"select sad.sadid, sad.saddtaditivo, sad.sadvlraditivo, sad.sadjustificativa, sad.saddttermino, sad.tpaid  from sisplan.situacaoaditivo sad
		where sad.sadid = %d ",
		(integer) $sadid
	);
	$rs = $db->pegaLinha( $sql );
	if ( is_array( $rs ) ) {
		return $rs;
	}
	return null;
}

function verifica_estado_pa( $atiid )
{
	global $db;
	if ( !$attid )
		return true;

	$sql = "
	SELECT
		ed.esdid
	FROM pde.atividade a
	LEFT JOIN workflow.documento d
		ON a.docid = d.docid
	LEFT JOIN workflow.estadodocumento ed
		ON d.esdid = ed.esdid
	WHERE
		a.atiid = ".$atiid;

	$esdid = $db->pegaUm($sql);

	if ( (int)$esdid <= (int)WORKFLOW_AGUARDANDO_PI )
		return true;
	else
		return false;
}


function verificaExistePI( $atiid )
{
	global $db;

	$sql = "SELECT atinumeropi FROM pde.atividade WHERE atiid = ".$atiid;

	$atinumeropi = $db->pegaUm($sql);

	if ( $atinumeropi == null )
		return false;
	else
		return true;
}

function verificaUsuarioAcacod ( $atiid )
{
	global $db;

	if ( $db->testa_superuser() )
		return true;

	if ( usuario_possui_perfil(PERFIL_COORDENADOR_ACAO_SIGPLAN) )
	{
		$sql = "
		SELECT
			ur.acacod
		FROM sisplan.usuarioresponsabilidade ur
		WHERE
			ur.usucpf = '".$_SESSION['usucpf']."'
		AND ur.acacod is not null
		AND ur.rpustatus = 'A'";
		$acacodArr = (array) $db->carregarColuna($sql);

		$acacodPA = $db->pegaUm("SELECT
									acacod
								 FROM
									monitora.acao
								 WHERE
									acaid = (SELECT
												acaid
											 FROM
												pde.atividade
											 WHERE
												atiid = ".$atiid.")
								");

		$flag = false;
		foreach ( $acacodArr as $acacodUsu )
		{
			if ( $acacodUsu == $acacodPA )
			{
				$flag = true;
				break;
			}
		}

		return $flag;
	}
	else
		return false;
}

function verificaUsuarioAcacodDescentralizado( $atiid )
{
	global $db;
	if ( verificaUsuarioAcacod( $atiid ) && verifica_tipo_autor_pa_descentralizado( $atiid ) )
		return true;
	else
		return false;
 }

 function verificaUsuarioAcacodCentralizado( $atiid )
{
	global $db;
	if ( verificaUsuarioAcacod( $atiid ) && verifica_tipo_autor_pa_descentralizado( $atiid ) )
		return true;
	else
		return false;
 }

 function verificaUsuarioDepid ( $atiid )
{
	global $db;

	if ( usuario_possui_perfil(PERFIL_DIRETOR_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_APOIO_DIRETOR_DEPARTAMENTO) )
	{
		$sql = "
		SELECT
			ur.depid
		FROM sisplan.usuarioresponsabilidade ur
		WHERE
			ur.usucpf = '".$_SESSION['usucpf']."'
		AND ur.depid is not null
		AND ur.depid in (SELECT
									depid
								 FROM
									pde.atividade
								 WHERE
									atiid = ".$atiid.")
		AND ur.rpustatus = 'A'";

		// se o usuario é do departamento retorna true se o departamento estiver nas responsabilidades dele
		return (bool)$db->pegaUm( $sql );
	}
	// se o usuário é superuser retorna true tb
	return $db->testa_superuser();
}

 function enviaEmailDescentralizacao( $atiid, $titulo, $mensagem )
{
	global $db;
	$titulo = "Descentralização Realizada - {$titulo}";
	$emailResponsaveisTmp = array();

	//PESSOA QUE ELABOROU
	$sql = "
			SELECT
				u.usunome,
				u.usuemail
			FROM
				pde.atividade a
			JOIN
				seguranca.usuario u ON u.usucpf = a.usucpf -- AND u.usustatus = 'A'
			WHERE
				a.atiid = {$atiid}";
	$emailResponsaveisTmp[] = $db->carregar($sql);

	//UNIDADE
	$sql = "
			SELECT
				u.usunome,
				u.usuemail
			FROM
				pde.atividade a
			JOIN
				planointerno.unidadeexecutora ue ON ue.uexid = a.uexid
			JOIN
				sisplan.usuarioresponsabilidade ur ON ur.uexid = ue.uexid AND ur.rpustatus = 'A'
			JOIN
				seguranca.usuario u ON u.usucpf = ur.usucpf -- AND u.usustatus = 'A'
			JOIN
				seguranca.perfilusuario pu ON pu.usucpf = u.usucpf AND pu.pflcod = ".PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA."
			WHERE
				a.atiid = {$atiid}";
	$emailResponsaveisTmp[] = $db->carregar($sql);

	//COORDENADOR DO PPA
	$sql = "
			SELECT
				u.usunome,
				u.usuemail
			FROM
				pde.atividade a
			JOIN
				monitora.acao aca ON aca.acaid = a.acaid
			JOIN
				sisplan.usuarioresponsabilidade ur ON ur.acacod = aca.acacod AND ur.rpustatus = 'A'
			JOIN
				seguranca.usuario u ON u.usucpf = ur.usucpf -- AND u.usustatus = 'A'
			JOIN
				seguranca.perfilusuario pu ON pu.usucpf = u.usucpf AND pu.pflcod = ".PERFIL_COORDENADOR_ACAO_SIGPLAN."
			WHERE
				a.atiid = {$_REQUEST['atiid']}";
	$emailResponsaveisTmp[] = $db->carregar($sql);


	//COORDENADOR DE PLANEJAMENTO
	//COORDENADOR DE ORÇAMENTO
	//APOIO AO COORDENADOR DE PLANEJAMENTO
	//APOIO AO COORDENADOR DE ORÇAMENTO
	//COORDENADOR GERAL DE PLANEJAMENTO E ORÇAMENTO
	$sql = "
			SELECT
				u.usunome,
				u.usuemail
			FROM
				seguranca.usuario u
			JOIN
				seguranca.perfilusuario p ON u.usucpf = p.usucpf
			WHERE
				(p.pflcod = ".PERFIL_COORDENADOR_PLANEJAMENTO."
			  OR p.pflcod = ".PERFIL_COORDENADOR_ORCAMENTO ."
			  OR p.pflcod = ".PERFIL_APOIO_COORDENADOR_PLANEJAMENTO."
			  OR p.pflcod = ".PERFIL_APOIO_COORDENADOR_ORCAMENTO."
			  OR p.pflcod = ".PERFIL_COORDENADOR_GERAL_PLANEJAMENTO_ORCAMENTO.")";

	$emailResponsaveisTmp[] = $db->carregar($sql);

	//RESPONSAVEL DEPARTAMENTO
	$sql = "
			SELECT
				u.usunome,
				u.usuemail
			FROM
				pde.atividade a
			JOIN
				sisplan.departamento dep ON dep.depid = a.depid
			JOIN
				sisplan.usuarioresponsabilidade ur ON ur.depid = dep.depid AND ur.rpustatus = 'A'
			JOIN
				seguranca.usuario u ON u.usucpf = ur.usucpf -- AND u.usustatus = 'A'
			JOIN
				seguranca.perfilusuario pu ON pu.usucpf = u.usucpf AND pu.pflcod = ".PERFIL_DIRETOR_DEPARTAMENTO."
			WHERE
				a.atiid = {$atiid}";

	$emailResponsaveisTmp[] = $db->carregar($sql);

	//trata o array de responsáveis
	foreach ($emailResponsaveisTmp as $responsavel)
		if($responsavel)
			foreach ($responsavel as $responsavelEmail)
				$responsaveisEmail["{$responsavelEmail['usuemail']}"] = array('usunome' => $responsavelEmail['usunome'],
																			  'usuemail' => $responsavelEmail['usuemail']);
	//envia os emails
	foreach ($responsaveisEmail as $email)
		email($email['usunome'], $email['usuemail'], $titulo, $mensagem, $cc='',$cco='');
}

function verificaAtividadePlanejamento($atiid){
    global $db;
    $sql = "SELECT
                pltid
            FROM
                pde.atividade
            WHERE
                atiid = '{$atiid}';";
    $retorno = $db->pegaUm($sql);

    if($retorno === false || $retorno == ''){
        return false;
    }else{
        return true;
    }
}

function verificaAtividadeSemPlanejamento($atiid){
    global $db;
    $sql = "SELECT
                pltid
            FROM
                pde.atividade
            WHERE
                atiid = '{$atiid}';";
    $retorno = $db->pegaUm($sql);

    if($retorno === false || $retorno == ''){
        return true;
    }else{
        return false;
    }
}

?>