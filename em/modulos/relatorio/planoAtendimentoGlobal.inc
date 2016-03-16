<?php
ini_set('memory_limit','128M');
if($_REQUEST['requisicao'] == 'gerarPdf'){
	
	$ano = $_SESSION["exercicio"];
	$anoAnterior = $ano -1;
	
	$sql = "SELECT 
				CASE 
	    			when ed.esddsc is null then 'Não Iniciado' 
	    			else ed.esddsc 
	    		end as esddsc, 
	    		CASE
                	WHEN ed.esdid is null THEN 0
                	ELSE ed.esdid
                END AS esdid, 
                count(*) 
			FROM 
				em.emiensinomedioinovador m
			INNER JOIN entidade.endereco        ent ON ent.entid        = m.entid
			INNER JOIN territorios.estado       est ON upper(est.estuf) = upper(ent.estuf)
			LEFT  JOIN workflow.documento       d   on d.docid          = m.docid
			LEFT  JOIN workflow.estadodocumento ed  on ed.esdid         = d.esdid 
			WHERE 
				m.entcodent NOT IN (select mem.entcodent
			                          from em.emiensinomedioinovador mem
			                          where mem.emianoreferencia = ".$anoAnterior."
			                          and   mem.emistatus        = 'A')
				AND   m.emianoreferencia = ".$ano."
				AND   m.emistatus        = 'A'
				AND   est.estuf          = '{$_REQUEST['estuf']}'
				AND   ed.esddsc          not in ('Finalizado', 'Concluido')
			GROUP BY ed.esddsc, ed.esdordem, ed.esdid
			ORDER BY ed.esdordem";
	
	$sql2 = "SELECT est.estdescricao as descricao,
			       d.esdid as esdid,
			       est.estuf as estuf,
			       count(*) as count
			FROM em.emiensinomedioinovador emi
				INNER JOIN entidade.endereco        ent ON ent.entid        = emi.entid
				INNER JOIN territorios.estado       est ON upper(est.estuf) = upper(ent.estuf)
				LEFT  JOIN workflow.documento       d   on d.docid          = emi.docid
				LEFT  JOIN workflow.estadodocumento ed  on ed.esdid         = d.esdid
			WHERE 
				d.esdid |agrupador|
				AND   emi.entcodent not in (select mem.entcodent
				                            from em.emiensinomedioinovador mem
				                            where mem.emianoreferencia = ".$anoAnterior."
				                            and   mem.emistatus        = 'A')
				AND   emi.emianoreferencia = ".$ano."
				AND   emi.emistatus        = 'A'
				AND   est.estuf            = '{$_REQUEST['estuf']}'
				AND   ed.esddsc            not in ('Finalizado', 'Concluido')
			GROUP BY d.esdid, est.estuf, descricao
			ORDER BY descricao";
	ver($sql2);
	$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "campo" => array("esddsc","estdescricao"), "get" => array("esdid","estuf"), "arrOff" => array("estuf"), "exibeLink" => array("descricao"));
	// Caso existam RPCs pendentes, não emite o PDF
	if(qtRPCPendentes('tabela_2',$sql,$sqlAgrupador)){
		$posacao = "document.location.href = 'em.php?modulo=relatorio/planoAtendimentoGlobal&acao=A&estuf=".$_REQUEST['estuf']."';";
		if( $_REQUEST['req'] == 'imprimir' ){
			$posacao = 'window.close();';
		}
		echo "  <script>
					alert('Todos os PRCs deverão estar com o status finalizado para emissão do relatório Plano de Atendimento Global.');
					$posacao
				</script>";
		exit;
	}else{
	// Caso NÃO existam PRCs pendentes, emite o PDF e tramita todos os PRCs para o status "Concluído"
		
		$sql = "SELECT d.docid
				FROM em.emiensinomedioinovador m
					INNER JOIN entidade.endereco        ent ON ent.entid        = m.entid
					INNER JOIN territorios.estado       est ON upper(est.estuf) = upper(ent.estuf)
					LEFT  JOIN workflow.documento       d   on d.docid          = m.docid
					LEFT  JOIN workflow.estadodocumento ed  on ed.esdid         = d.esdid 
				WHERE m.entcodent not in (select mem.entcodent
										  from em.emiensinomedioinovador mem
										  where mem.emianoreferencia = ".$anoAnterior."
										  and   mem.emistatus        = 'A')
				AND   m.emianoreferencia = ".$ano."
				AND   m.emistatus        = 'A'
				AND   est.estuf          = '{$_REQUEST['estuf']}'
				AND   ed.esddsc          <> 'Concluído'";
		$rsDocs = $db->carregar($sql);
		
		require_once APPRAIZ . 'includes/workflow.php';
		foreach ($rsDocs as $linha){
			if(!wf_alterarEstado($linha['docid'], AED_EMITIR_PLANO_ATENDIMENTO_GLOBAL, '', array())){
				$falhou = true;
				exit;
			}
		}
		if( $falhou ){
			$posacao = "document.location.href = 'em.php?modulo=relatorio/planoAtendimentoGlobal&acao=A&estuf=".$_REQUEST['estuf']."';";
			if( $_REQUEST['req'] == 'imprimir' ){
				$posacao = 'window.close();';
			}
			echo "<script>
						alert('Não foi possível concluir o PRC, por favor tente novamente.');
						alert('$posacao');
						$posacao
					</script>";
			die();
		}
	}
	
	$htm = '<style>
	
			.subtitulodireita{
			    FONT-SIZE: 8pt;
			    COLOR: black;
			    FONT-FAMILY: Arial, Verdana;
			    TEXT-ALIGN: right;
				BACKGROUND-COLOR: #dcdcdc;
			}
			.tabela{
			    FONT-SIZE: xx-small;
			    BORDER-RIGHT: #cccccc 1px solid;
			    BORDER-TOP: #cccccc 1px solid;
			    BORDER-LEFT: #cccccc 1px solid;
			    BORDER-BOTTOM: #cccccc 1px solid;
				TEXT-DECORATION: none;
				WIDTH: 95%;
				TEXT-COLOR: #000000;
			}		
			.subtituloCentro{
			    FONT-WEIGHT: bold;
			    FONT-SIZE: 8pt;
			    COLOR: black;
			    FONT-FAMILY: Arial, Verdana;
			    BACKGROUND-COLOR: #f0f0f0;
			    TEXT-ALIGN: center
			}	
			table.listagem{
				font-size: 11px;
				padding: 3px;			
				border-top: 2px solid #404040;
				border-bottom: 3px solid #dfdfdf;			
				border-collapse: collapse;
			}	
			table.listagem P{
				font-size: 11px;
				margin: 0px;
				padding: 3px;		
			}	
			table.listagem P.marcado{color: #D90000;}

			table.listagem IMG{margin-left: 0px;}
				
			table.listagem thead{background-color: #E3E3E3;}
		
			table.listagem thead P{color: #6A6A6A;}

			table.listagem thead P B{color: #D90000;}
	
			table.listagem thead td{
				border-right: 1px solid white;
				padding: 2px;
				padding-left: 3px;
			}
			
			table.listagem thead td.order{
				background-color: #133368;
				border-right: 1px solid #ff9000;
			}

			table.listagem thead td A, 
			table.listagem thead td A:visited{
				color: #6A6A6A;
			}

			table.listagem thead td A:hover{color: red;}

			table.listagem thead td.order A{color: white;}

			table.listagem thead td.order A:visited{color: #BFBFBF;}
	
			table.listagem tbody td{
				border-right: 1px solid #f1f1f1;
				border-left: 1px solid white;
				border-collapse: collapse;
				border: 1px solid #ffffff;
			}

			table.listagem tbody td P A{color: #133368;}

			table.listagem tbody td A:visited{color: black;}

			table.listagem tbody td A:hover, 
			table.listagem tbody td A:visited:hover{color: #E47100;}

			table.listagem tbody td P A.alerta, 
			table.listagem tbody td P A.alerta:visited{
				color: #E47100;
				font-weight: bold;
			}

			table.listagem tbody td P A.alerta:hover, 
			table.listagem tbody td P A.alerta:visited:hover{
				color: black;
				font-weight: bold;
				text-decoration: none;
			}

			table.listagem tbody TR.marcado{background-color: #f5f5f5;}
			
		  </style>';
	
	$htm .= monta_titulo2("Plano de Atendimento Global", "Dados da Secretaria");

}else{
	
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br/>";	
	
	monta_titulo("Plano de Atendimento Global", "Dados da Secretaria");
}


if($_REQUEST['estuf']){

	$sql = "select distinct entnumcpfcnpj, ent.entnome, ede.estuf, mundescricao from entidade.entidade ent
			inner join entidade.endereco ede on ent.entid = ede.entid
			inner join territorios.municipio mun on mun.muncod = ede.muncod
			inner join entidade.funcaoentidade fet ON ent.entid = fet.entid and fet.funid = 6 
			where ede.estuf = '{$_REQUEST['estuf']}'
			and fuestatus = 'A'
			and entstatus = 'A'";
	
	$dadosSecretaria = $db->pegaLinha($sql);
	
	$sql = "select * from entidade.entidade ent 
			inner join entidade.endereco ede on ede.entid = ent.entid 
			inner join entidade.funcaoentidade fue on fue.entid = ent.entid and fue.funid = 25
			where ede.estuf = '{$dadosSecretaria['estuf']}'
			and ede.endstatus = 'A'
			and ent.entstatus = 'A'
			and fue.fuestatus = 'A'
			order by ent.entid desc";
	
	$dadosDirigente = $db->pegaLinha($sql);
	
	$htm .= "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
	$htm .= "<tr><td class='subtitulodireita'>CNPJ:</td><td>".formatar_cnpj($dadosSecretaria['entnumcpfcnpj'])."</td></tr>";
	$htm .= "<tr><td class='subtitulodireita'>Secretaria:</td><td>{$dadosSecretaria['entnome']}</td></tr>";
	$htm .= "<tr><td class='subtitulodireita'>Município:</td><td>{$dadosSecretaria['mundescricao']} - {$dadosSecretaria['estuf']}</td></tr>";
//	$htm .= "<tr><td class='subtitulodireita'>CPF do Dirigente:</td><td>".formatar_cpf($dadosDirigente['entnumcpfcnpj'])."</td></tr>";
//	$htm .= "<tr><td class='subtitulodireita'>Nome do Dirigente:</td><td>{$dadosDirigente['entnome']}</td></tr>";
	$htm .= "<tr><td class='subtitulodireita'>Valor Total do Repasse:</td><td id=\"vlr_total_estado\">{valor_total}</td></tr>";
	$htm .= "</table>";
	
	$htm .= monta_titulo2("", "Dados das Escolas");

	$sql = "SELECT DISTINCT 
				ei.emiid,
				et.entid,
			 	et.entcodent,
				et.entnome,
				mu.mundescricao,
				mu.estuf,
				tj.etjjornada,
				ei.etjid 
			FROM 
				em.emiensinomedioinovador ei
			INNER JOIN entidade.entidade 	 et ON ei.entid = et.entid
			INNER JOIN entidade.endereco 	 ed ON ed.entid = et.entid
			INNER JOIN territorios.municipio mu ON mu.muncod = ed.muncod
			LEFT  JOIN em.emitipojornada 	 tj ON tj.etjid = ei.etjid
			INNER JOIN em.emigap 			gap ON gap.emiid = ei.emiid
			WHERE 
				ed.estuf = '{$_REQUEST['estuf']}'
				AND ei.emistatus        = 'A'";
	
	$rsEscolas = $db->carregar($sql);
	$rsEscolas = $rsEscolas ? $rsEscolas : array();
	
	$valorTotalEstado=0;

	$htm .= "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
	foreach($rsEscolas as $escola){
		
		$qtd++;
		
		$sql = "SELECT * FROM em.emicenso WHERE entid = '".$escola['entid']."' order by emcserie asc";
		$rsCenso = $db->carregar($sql);
		
		$arCenso = array();
		$totalAluno = 0;
		$rsCenso = $rsCenso ? $rsCenso : array();		
		foreach($rsCenso as $censo){
			$arCenso[] = $censo['emcserie']."ª série ".$censo['emcquantidadealunos']." alunos";
			$totalAluno += $censo['emcquantidadealunos'];
		}			
		$htmlCenso = count($arCenso) ? implode(', ',$arCenso) : 'Sem alunos';
				
		/*
		 * REPASSE CAPITAL E CUSTEIO
		 */
		
		$sql = "SELECT * FROM em.emicenso WHERE entid = '".$escola['entid']."' order by emcserie asc";
		$rsCenso = $db->carregar($sql);
		
		$totalAluno = 0;
		$rsCenso = $rsCenso ? $rsCenso : array();		
		foreach($rsCenso as $censo){
			$arCenso[] = $censo['emcserie']."ª série ".$censo['emcquantidadealunos']." alunos";
			$totalAluno += $censo['emcquantidadealunos'];
		}			
		$htmlCenso = count($arCenso) ? implode(', ',$arCenso).", Total de alunos: {$totalAluno}" : 'Sem alunos';
				
		// Repasse total
		$valorRepasse = 0;
		if(!empty($escola['etjjornada'])){
			$sql = "SELECT * FROM em.emireccusteiocapital WHERE emrjornada = {$escola['etjjornada']} AND {$totalAluno} BETWEEN emrqtdinialunos AND emrqtdfinalunos;";
			$reccusteiocapital = $db->pegaLinha($sql);
			$valorRepasse = !empty($reccusteiocapital['emrcusteio']) && !empty($reccusteiocapital['emrcusteio']) ? $reccusteiocapital['emrcusteio']+$reccusteiocapital['emrcapital'] : 0;
		}
		$valorTotalEstado += $valorRepasse;
				
		/*
		 * FIM REPASSE
		 */
		
		$htm .= "<tr>";
		$htm .= "<td class='subtitulodireita'>Escola:</td><td>{$escola['entcodent']} - {$escola['entnome']}</td>";
		$htm .= "</tr><tr>";
		$htm .= "<td class='subtitulodireita'>Município:</td><td>".$escola['mundescricao'].'/'.$escola['estuf']."</td>";
		$htm .= "</tr><tr>";
		$htm .= "<td class='subtitulodireita'>N° total de alunos beneficiados:</td><td>{$totalAluno} alunos</td>";
		$htm .= "</tr><tr>";
		$htm .= "<td class='subtitulodireita'>Jornada:</td><td>".($escola['etjjornada'] ? $escola['etjjornada'].' horas' : 'não informado')."</td>";
		$htm .= "</tr><tr>";
		$htm .= "<td class='subtitulodireita'>Valor do Repasse:</td><td>R$ ".number_format($valorRepasse, 2, ",", ".")."</td>";
		$htm .= "</tr>";	
			
		$sql = "select distinct
					mcp.mcpid,
					mcp.mcpdsc 
				from 
					em.macrocampo mcp
				inner join 
					em.emigap gap on mcp.mcpid = gap.mcpid
				where 
					mcp.mcpstatus = 'A' 
				and 
					gap.emiid = {$escola['emiid']}
				order by 
					mcp.mcpid";
		
		$arrMacroCampos = $db->carregar($sql);
		$arrMacroCampos = $arrMacroCampos ? $arrMacroCampos : array();
		
		foreach($arrMacroCampos as $dados){			
			
			$htm .= "<tr><td colspan='2' class='subtituloCentro'>Macrocampo: {$dados['mcpdsc']}</td></tr>";				
			$htm .= "<tr><td colspan='2'>";
			
			$sql = "select papcaoatividade, papmeta from em.emigap where emiid = {$escola['emiid']} and mcpid = {$dados['mcpid']}";
			//$cabecalho = array('Ação/Atividade','Meta');			
			//$db->monta_lista_simples($sql, $cabecalho, 100, 10, 'N', '95%', 'N', false, array('50'), 50);
			//$db->monta_lista($sql, $cabecalho, 100, 10, 'N', '', '', 'form'.$escola['emiid'].$dados['mcpid'], array('50%', '50%'));
			
			$rsAtividades = $db->carregar($sql);
			if($rsAtividades){
				
				$htm .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<thead>
							<tr>
								<th>Ação/Atividade</th>
								<th>Meta</th>								
							</tr>
						</thead>
						<tbody>';
				
				foreach($rsAtividades as $atividade){
					$htm .= '<tr>
							 <td width="50%">'.$atividade['papcaoatividade'].'</td>
							 <td width="50%">'.$atividade['papmeta'].'</td>
						   </tr>';
				}
				
				$htm .= '</tbody></table>';
			}
			
			
			$htm .= "</td></tr>";
		}
		$htm .= "<tr><td colspan='2'>&nbsp;</td></tr>";
	}
	$htm .= "</table>";
	$htm = str_replace('{valor_total}', 'R$ '.formata_valor($valorTotalEstado,2), $htm);

	if($_REQUEST['requisicao'] == 'gerarPdf'){
		
		$htm .= '<p>&nbsp;</p>
				<table width="95%" align="center" style="border: 1px solid black;">
					<tr>
						<td colspan="3" bgcolor="#f0f0f0" style="border-bottom: 1px solid black;">Aprovação</td>
					</tr>
					<tr>
						<td width="50%" height="200" valign="bottom" align="center" style="border-right: 1px solid black;"><hr style="width: 95%;" />LOCAL E DATA</td>						
						<td width="50%" valign="bottom" align="center"><hr style="width: 95%;" />SECRETÁRIO DE ESTADO DA EDUCAÇÃO </td>
					</tr>
				</table>';
		
		if( $_REQUEST['req'] != 'imprimir' ){
			ob_clean();
			$content = http_build_query( array('conteudoHtml'=> utf8_encode($htm) ) );
			$context = stream_context_create(array( 'http'    => array(
									                'method'  => 'POST',
									                'content' => $content ) ));
			$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
			header('Content-Type: application/pdf');
			header("Content-Disposition: attachment; filename=plano_atendimento_global");
			echo $contents;
			exit;
		}
	}
	if( $_REQUEST['req'] == 'imprimir' ){
		echo '<a onclick="print();" style="color:blue"><u>Imprimir</u></a>';
	}
	echo '<div id="impressao">'.$htm.'</div>';
	if( $_REQUEST['req'] == 'imprimir' ){
		exit;
	}
	?>	
	<script>
	function imprimir(pg)
	{
		var oJan;
	    oJan    = window.open(pg,'Janela','toolbar=yes,location=no,directories=yes,status=yes,menubar=yes,scrollbars=yes,resizable=yes,width=800,height=600');
	    oJan.history.go();
	}
	function gerarPdf()
	{
		document.location.href = 'em.php?modulo=relatorio/planoAtendimentoGlobal&acao=A&requisicao=gerarPdf&estuf=<?php echo $_REQUEST['estuf'] ?>';
	}
	</script>
	<center>
	<p>
		<input type="button" value="Gerar PDF" onclick="gerarPdf()"/>
		<input type="button" value="Imprimir" onclick="imprimir('em.php?modulo=relatorio/planoAtendimentoGlobal&acao=A&requisicao=gerarPdf&req=imprimir&estuf=<?php echo $_REQUEST['estuf'] ?>')"/>
	</p>
	</center>
	
<?php }else{ ?>
	
	<script>
	function abreDados(estuf)
	{
		document.location.href = 'em.php?modulo=relatorio/planoAtendimentoGlobal&acao=A&estuf='+estuf;
	}
	</script>
	<?php
	if(!checkPerfil(array(PERFIL_ADMINISTRADOR, PERFIL_SUPER_USUARIO))){
		$stJoin = " INNER JOIN em.usuarioresponsabilidade eu ON eu.estuf = te.estuf ";
		$stWhere = " WHERE rpustatus = 'A' AND usucpf = '{$_SESSION["usucpf"]}' ";
	}
	
	$sql = "SELECT
				'<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abreDados( \''|| te.estuf ||'\' );\"></center>' as codigo,
				estdescricao as descricao
			FROM
				territorios.estado te
			{$stJoin}	
			{$stWhere} 
			ORDER BY
				te.estuf";
	
	$cabecalho = array( "Ação", "Estado" );
	$db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
}
?>
<script>
	document.getElementById('vlr_total_estado').innerHTML = 'R$ <?php echo number_format($valorTotalEstado, 2, ",", ".") ?>';
</script>
