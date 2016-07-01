<?php

//if($_REQUEST['xls'] == true){
	$sql = "
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
		a.atiorcamentocusteio + a.atiorcamentocapital as atiorcamento,
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
		a.atinotacredito,
		ptres.ptres,
		to_char( a.atidatacadastro, 'DD/MM/YYYY') as atidatacadastro,
		esd.esddsc as estadodocumento
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
		LEFT JOIN monitora.acao aca ON aca.acaid = a.acaid
		LEFT JOIN sisplan.ptres ptres on ptres.exercicio = a.atianopi
									    AND ptres.prgcod = aca.prgcod
									    AND ptres.acacod = aca.acacod
									    AND ptres.unicod = aca.unicod
									    AND ptres.loccod = aca.loccod
									    AND ptres.ptrstatus = 'A'
		LEFT JOIN workflow.documento doc ON doc.docid = a.docid
		LEFT JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
		WHERE a.atianopi = '{$_SESSION['exercicio']}'";
	$dadoPAs = $db->carregar($sql);
	$dadosTodasPAs = array();
	foreach ($dadoPAs as $index=>$value){
		extract($value);
		$dadosPA = array();

	/////////////////////////////////PROJETO////////////////////////////////////////
//		if ( $atistatuspi == 'R' )
//		{
//			array_push($dadosPA, array(
//								 "label" => "Motivo da Recusa",
//								 "valor" => $atijustificativarecusa)
//				  );
//		}
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
//		if ( $atiprotocolo ){
			array_push($dadosPA, array(
									 "label" => "Número do Processo IPHAN",
									 "valor" => $atiprotocolo)
					  );
//		}
//		if ( $atinumeropi ){
			array_push($dadosPA, array(
									 "label" => "Número do PI",
									 "valor" => $atinumeropi)
					  );
//		}
//		if ( $atinotacredito ){
			array_push($dadosPA, array(
									 "label" => "Nota de Crédito",
									 "valor" => $atinotacredito)
					  );
//		}
		array_push($dadosPA, array(
								 "label" => "Título",
								 "valor" => $atidescricao)
				  );
		array_push($dadosPA, array(
								 "label" => "Estado Atual do Documento",
								 "valor" => $estadodocumento)
				  );
		array_push($dadosPA, array(
								 "label" => "Data de Cadastro",
								 "valor" => $atidatacadastro)
				  );
		array_push($dadosPA, array(
								 "label" => "Descrição",
								 "valor" => $atidetalhamento)
				  );
	/////////////////////////////////FIM////////////////////////////////////////

	/////////////////////////////////RESPONSÁVEIS////////////////////////////////////////
		array_push($dadosPA, array(
								 "label" => "",
								 "valor" => 'Responsáveis')
				  );

		//UNIDADE
		$sql = "
				SELECT
					u.usunome
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
		array_push($dadosPA, array(
								 "label" => "Unidade",
								 "valor" => formataResponsaveis($db->carregarColuna($sql)))
				  );

		//COORDENADOR DO PPA
		$sql = "
				SELECT
					u.usunome
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
					a.atiid = {$atiid}";

		array_push($dadosPA, array(
								 "label" => "Coordenador de ação no PPA",
								 "valor" => formataResponsaveis($db->carregarColuna($sql)))
				  );

		//RESPONSAVEL DEPARTAMENTO
		$sql = "
				SELECT
					u.usunome
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

		array_push($dadosPA, array(
								 "label" => "Departamento",
								 "valor" => formataResponsaveis($db->carregarColuna($sql)))
				  );

		//COORDENADOR DE PLANEJAMENTO
		$sql = "
				SELECT
					u.usunome
				FROM
					seguranca.usuario u
				JOIN
					seguranca.perfilusuario p ON u.usucpf = p.usucpf
				WHERE p.pflcod = ".PERFIL_COORDENADOR_PLANEJAMENTO." -- AND u.usustatus = 'A'";

		array_push($dadosPA, array(
								 "label" => "Coordenador de planejamento",
								 "valor" => formataResponsaveis($db->carregarColuna($sql)))
				  );

		//COORDENADOR DE ORÇAMENTO
		$sql = "
				SELECT
					u.usunome
				FROM
					seguranca.usuario u
				JOIN
					seguranca.perfilusuario p ON u.usucpf = p.usucpf
				WHERE p.pflcod = ".PERFIL_COORDENADOR_ORCAMENTO ." -- AND u.usustatus = 'A'";

		array_push($dadosPA, array(
								 "label" => "Coordenador de orçamento",
								 "valor" => formataResponsaveis($db->carregarColuna($sql)))
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
							 "label" => "PTRES",
							 "valor" => $ptres)
			  );

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
//	if ( $atipronacquest == 't' )
//	{
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
//	}
//	else
//	{
//		array_push($dadosPA, array(
//								 "label" => "",
//								 "valor" => "PRONAC - NÃO")
//				  );
//	}
	/////////////////////////////////FIM////////////////////////////////////////


	/////////////////////////////////EMENDA////////////////////////////////////////
//	if ( $atiemenda == 't' )
//	{
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
//	}
//	else
//	{
//		array_push($dadosPA, array(
//								 "label" => "",
//								 "valor" => "Emenda - NÃO")
//				  );
//	}
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

		// Orçamento por natureza
		$cabecalho = array( 'Código', 'Valor');
		$sql = "SELECT
					ctecod||''||gndcod||''||mapcod||''||edpcod AS natcod ,
					opavalor
				FROM
					pde.orcamentopa opa
				JOIN
					naturezadespesa ndp ON ndp.ndpid = opa.ndpid
				WHERE
					atiid = ".$atiid;

		$arOrcaNatureza = $db->carregar($sql);
		$arOrcaNatureza = $arOrcaNatureza ? $arOrcaNatureza : array();

		$out = '
		<table width="100%" bgcolor="#ffffff" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
			<thead>
				<tr>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Código</td>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor</td>
				</tr>
			</thead>
			<tbody>';
		foreach ( $arOrcaNatureza as $campo )
		{
			$out .= '
				<tr bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';">
					<td align="right" style="color:#999999;" title="Código">'.$campo['natcod'].'</td>
					<td align="right" style="color:#999999;" title="Valor">'.number_format( $campo['opavalor'], 2, ",", "." ).'<br></td>
				</tr>';
			$total += $campo['opavalor'];
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
								 "label" => "Orçamento por Natureza R$",
								 "valor" => $out )
				  );


		// Orçamento por fonte
		$total = 0;

		$cabecalho = array( 'Código', 'Valor');
		$arOrcaFontes = $db->carregar("SELECT fonid, opfvalor FROM pde.orcamentopafonte WHERE atiid = ".$atiid);
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
					<td align="right" style="color:#999999;" title="Código">'.$campo['foncod'].'</td>
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

		if ( (int)$estadoAtual['esdid'] == (int)WORKFLOW_AGUARDANDO_PI )
		{
			$html .= "
			<tr>
				<td bgcolor='#CCCCCC' width='30%' align='left'>Número do PI:</td>
				<td bgcolor='#DFDFDF'>
				<form action='' name='formulario' method='post'>
					<input type='text' title='' onblur='MouseBlur(this);' onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal'  value='$atinumeropi' maxlength='9' size='26' name='atinumeropi'/>
					<input type='button' name='botao_gravar' value='Gravar' onclick='gravar();'/>
				</form>
				</td>
			</tr>";
		}

		/////////////NOTA DE CREDITO//////////////////
		if ( (int)$estadoAtual['esdid'] == (int)EM_ANALISE_COORDENACAO_ORCAMENTO_FINANCAS || (int)$estadoAtual['esdid'] == (int)WORKFLOW_AGUARDANDO_DESCENTRALIZACAO )
		{

			$html .= "
			<tr>
				<td bgcolor='#CCCCCC' width='30%' align='left'>Nota de Crédito:</td>
				<td bgcolor='#DFDFDF'>
				<form action='' name='formulario' method='post'>
					<input type='text' title='' onblur='MouseBlur(this);' onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal'  value='$atinotacredito' maxlength='20' size='26' name='atinotacredito'/>
					<input type='button' name='botao_gravar' value='Gravar' onclick='gravar_nota();'/>
				</form>
				</td>
			</tr>";
		}
		/////////////FIM//////////////////
		$dadosTodasPAs[] = $dadosPA;
	}


	$html = "<table align='center' width='95%' border='0' cellpadding='2' cellspacing='1'>";

	$td = '';

	$i = 0;
	foreach ($dadosTodasPAs[0] as $value) {
		if($value['label'] != ''){
			$td .= "<td bgcolor='#DFDFDF'>" . $value['label'] . "</td>";
			$i++;
		}
	}

	$html .= "<tr><td colspan='{$i}' bgcolor='#CCCCCC'><center><b>Projeto</b></center></td></tr>";
	$html .= "<tr>{$td}</tr>";

	foreach ($dadosTodasPAs as $dadoPA) {
		$td = '';
		foreach ($dadoPA as $value)
			if($value['label'] != '')
				$td .= "<td>{$value['valor']}</td>";
		$html .= "<tr>{$td}</tr>";
	}

	$html .= "</table>";


		header( 'Content-Type: application/vnd.ms-excel' );
		header( 'Expires: 0' );
		header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
		header( 'Content-Disposition: attachment; filename="relatorio.xls"' );

		$output = preg_replace("/<img[^>]+\>/i", "", $html);

		$output = preg_replace("/<a[^>]+\>/i", "", $output);

		$output = str_replace('</a>','',$output);

		$output = str_replace('&nbsp;&nbsp;&nbsp;','',$output);

		print str_replace('<td align="center">&nbsp;&nbsp;','',$output);
//		die();
//	}else{
//		include APPRAIZ . 'includes/cabecalho.inc';
//		print '<br/>';
//		$db->cria_aba( $abacod_tela, $url, '' );
//		monta_titulo( $titulo_modulo, '' );
//		//echo $html;
//
//	}



function formataResponsaveis($rs){
	if(!$rs){
		return "-";
	}
	$out = "";
	foreach ($rs as $value){
		$out .= $value;
		if(next($rs))
			$out .= ", ";
	}
	return $out;
}

?>
<!--<table id="tbExportar" class="tabela" cellspacing="1" cellpadding="3" border="0" align="center" bgcolor="#f5f5f5">-->
<!--	<tr>-->
<!--		<td align="center">Exportar o resultado em <a href="#" onclick="imprime_rel();" >XLS</a></td>-->
<!--	</tr>-->
<!--</table>-->


<!--<script type="text/javascript">-->
<!--	function imprime_rel(cod)-->
<!--	{-->
<!--	    //Abre popup em branco-->
<!--	   	janela = window.open('about:blank',"relatorio","menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");-->
<!--	   	janela.location ="sisplan.php?modulo=principal/listarpa&acao=A&xls=true";-->
<!--	}-->
<!--</script>-->
