<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:500%; width:100%; opacity:0.5; filter:alpha(opacity=40); z-index:100 " >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<?php
header('content-type: text/html; charset=ISO-8859-1');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

function temPermissaoAnaliseFNDE()
{
	
	$arrPflcodsAnaliseFNDE = Array( PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR, PAR_PERFIL_EQUIPE_TECNICA );
	
	return possui_perfil($arrPflcodsAnaliseFNDE);
}

function bloqueiaParaConsultaEstMun()
{

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	if((in_array(  PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )))
	{
            return false;
	}
	elseif (in_array(  PAR_PERFIL_CONSULTA_ESTADUAL, $perfil ) || in_array(  PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA, $perfil ) )
	{
            return true;
	}
	else 
	{
            return false;
	}
}

if( ! $_SESSION['par']['muncod'] ){
			    	
	$estado = $_SESSION['par']['estuf'];
	
	$local = 'Estado: ' . $estado . '   ';      
}
else 
{
	$estado = $_SESSION['par']['estuf'];
    $codMun = $_SESSION['par']['muncod'];
	$sqlMun = "select e.muncod as codigo, e.mundescricao as descricao from territorios.municipio e where e.muncod = '".$codMun."' order by e.mundescricao asc";
	$munAtual = $db->carregar($sqlMun);
	$munAtual = $munAtual[0]['descricao'];
	$local = 'Município: ' . $munAtual . ' - ' . $estado;
}

// Id do documento
$dopid = $_REQUEST['dopid'];
$_SESSION['dopid'] = $dopid;

function getFinalizado($dopid = ''){
	
	global $db;
	
	if($dopid == '')
	{
		$dopid = $_SESSION['dopid'];	
	}
	
	$sqlFinalizado = 
	"
		SELECT 
			dp.dopacompanhamento 
		FROM par.documentopar dp 
		INNER JOIN
			(
				SELECT  
					 si.icoid, $dopid as dopid
				FROM
					par.subacao s
					INNER JOIN par.subacaodetalhe 					sd   ON sd.sbaid = s.sbaid
					INNER JOIN par.termocomposicao 					tc   ON tc.sbdid = sd.sbdid
					INNER JOIN par.subacaoitenscomposicao 			si   ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
					INNER JOIN par.subacaoitenscomposicaocontratos 	sicc ON  si.icoid = sicc.icoid
					INNER JOIN par.propostaitemcomposicao 			pic  ON pic.picid = si.picid
				WHERE 
					s.sbaid in (
						SELECT DISTINCT  
								s.sbaid
						FROM
							par.subacao s
						INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
						INNER JOIN par.termocomposicao 	tc ON tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
						INNER JOIN par.documentopar 	dp ON dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
						WHERE
							dp.dopid = $dopid
				)
		
			) as foo ON foo.dopid = dp.dopid
		WHERE 
			dp.dopid = $dopid
	";
	
	$result = $db->carregar($sqlFinalizado);
	if(is_array($result) && count($result))
	{
		$acompanhando = $result[0]['dopacompanhamento'];
		
		// Finalizado
		if( $acompanhando == 'f')
		{
			return 'finalizado';
			
		}
		else
		{
			
			return 'emacompanhamento';
		}
		
	}
	else
	{
		return false;
	}
	
}



//  function getNumDoc( $dopid )
//  {
//  	$sql = "SELECT  
// 				CASE WHEN dp2.dopano::boolean THEN
// 					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
// 				ELSE
// 					dp.dopnumerodocumento::text
// 				END
// 					as ndocumento 
// 			FROM par.documentopar dp
			
// 			LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
// 			WHERE dp.dopid = {$dopid}";
//  	global $db;
	 	
//  	$result = $db->carregar( $sql );
//  	if(is_array($result) && count($result))
//  	{
 		
// 		return $acoes.$result[0]['ndocumento'];
//  	}
//  	else
//  	{
//  		return 'erro';
//  	}
 	
 
//  }

$acoes = "<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" title=\"Visualizar Termo\" onclick=\"carregaTermoMinuta('".$dopid."');\" border=\"0\">";


// Caso seja a requisição ajax redireciona para a função carregaDadoItens 
if( $_REQUEST['requisicao'] == 'carregaDadosItens' )
{
	ob_clean();
	carregaDadoItens();
	exit();
}

// Caso seja a requisição ajax redireciona para a função carregaDadoItens
if( $_REQUEST['requisicao'] == 'carregaDadosItensMonitoramento' )
{
	ob_clean();
	carregaDadoItensMonitoramento();
	exit();
}

// Caso seja a requisição ajax redireciona para a função carregaDadoItens
if( $_REQUEST['requisicao'] == 'carregaItensAnaliseFNDE' )
{
	ob_clean();
	carregaItensAnaliseFNDE();
	exit();
}


if( $_REQUEST['requisicao'] == 'finalizaAcompanhamento')
{
	ob_clean();
	$dopid = $_REQUEST['dopid'];
	$finalizaOk = $_REQUEST['finalizaOk']; 
	if( $finalizaOk )
	{
			global $db;
			// Atualiza as informações do item na tabela par.subacaoitenscomposicao
			$sqlFinaliza = "
										
				UPDATE	
					par.documentopar 
				SET
					dopacompanhamento = FALSE
				WHERE
					dopid = {$dopid}
			";
			
			// Executa a query
			$db->executar($sqlFinaliza);	
				
		// Executa as queries
		$db->commit();
		die(' Acompanhamento finalizado com sucesso!');
	}	
	else
	{
		// Tratar o retorno p não redirecionar
		die(' Para finalizar é necessário anexar no mínimo 1 nota fiscal para cada item.'); 	
	}
}

// Caso seja a requisição ajax redireciona para a função carregaDadoItens
if( $_REQUEST['requisicao'] == 'cancelarMonitoramentoItem' )
{
	ob_clean();
	// Recebe uma string compactada com os resultados
	$icoId = $_REQUEST['icoId'];
	
	$sqlItem = " SELECT count(icoid) as qtd  from par.detalhamentoquantidadeitens where icoid = $icoId ";

	$qtdInseridas = $db->carregar($sqlItem);
	$qtdInseridas = $qtdInseridas[0]['qtd'];
	
	if( $qtdInseridas )
	{
		die("Não foi possível cancelar o monitoramento do item, pois o mesmo o já foi detalhado");
	}
	else 
	{
		$sqUpdate = 
			" UPDATE	
				par.subacaoitenscomposicao
			SET
				icomonitoramentocancelado	= true,
				icoquantidaderecebida 		= 0
			WHERE
				icoid 			= {$icoId}
		";
		
			// Executa a query
			$db->executar($sqUpdate);	

		// Executa as queries
		$db->commit();
		
		die("Monitoramento do item {$icoId} cancelado com sucesso.");
	}
}


// Caso seja a requisição ajax redireciona para a função carregaDadoItens
if( $_REQUEST['requisicao'] == 'ativaMonitoramentoItem' )
{
	ob_clean();
	// Recebe uma string compactada com os resultados
	$icoId = $_REQUEST['icoId'];
	
		$sqUpdate = 
			" UPDATE	
				par.subacaoitenscomposicao
			SET
				icomonitoramentocancelado	= false
			WHERE
				icoid 			= {$icoId}
		";
		
			// Executa a query
			$db->executar($sqUpdate);	

		// Executa as queries
		$db->commit();
		
		die("Monitoramento do item {$icoId} ativado com sucesso.");
	
}

// Caso seja a requisição ajax redireciona para a função carregaDadoItens
if( $_REQUEST['requisicao'] == 'SalvarItensMonitoramento' )
{
	ob_clean();
	// Recebe uma string compactada com os resultados
	$strParams = $_REQUEST['arrParams'];
	
	// Retira o "|" do final para transformar em array os valores dos campos
	$strParams = substr_replace($strParams, '', -1);
	
	// Cria um array com os valores dos campos
	$arrParams = explode('|', $strParams);
	// Inicia array com os itens
	$arrItensMonitoramento = array();
	
	// Cria o array com os códigos e com os valores
	foreach ( $arrParams as $k => $v )
	{
		$arrTemp = explode(',', $v);
		
		$arrItensMonitoramento[$k]['id'] = $arrTemp[0];
		$arrItensMonitoramento[$k]['valor'] = $arrTemp[1];
		
	}
	
	$contemErro = false;
	$itensComErro = array();
	
	// Salva valores
	foreach( $arrItensMonitoramento as $k => $v )
	{
		if( ($v['id']) && ($v['valor'] != '') )
		{
			
			// Caso o valor tenha sido deixado em branco ele coloca o valor de 0 para o insert/update 
			$valor = ( $v['valor'] ) ? $v['valor'] : 0;
			
			$sqlItem = " SELECT count(icoid) as qtd  from par.detalhamentoquantidadeitens where icoid = {$v['id']} ";
			
			$qtdInseridas = $db->carregar($sqlItem);
			$qtdInseridas = $qtdInseridas[0]['qtd'];
	
			
			if( $qtdInseridas > $valor )
			{
				$contemErro = true;
				$itensComErro[] = $v['id'];
			}
			else
			{
			    #Selecionar Item composicao
                            $sql = "INSERT INTO par.historicosubacaoitenscomposicao(
                                        icoid, icoano, icodescricao, icodatamonitoramento, icousucpfmonitoramento, 
                                        icoquantidade, icoquantidadetecnico, icoquantidaderecebida, icovalor
                                    )
                                    SELECT 
                                        icoid, icoano, icodescricao, icodatamonitoramento, icousucpfmonitoramento, 
                                        icoquantidade, icoquantidadetecnico, icoquantidaderecebida, icovalor
                                    FROM par.subacaoitenscomposicao
                                    WHERE icoid = {$v['id']};";
                            $db->executar($sql);
                            
                            // Caso tenha atribuido novamente valor para o item reativa ele.

                            $ativa = ( $valor > 0 ) ? ",icomonitoramentocancelado = false" : '';  

                            // Atualiza as informações do item na tabela par.subacaoitenscomposicao
                            $sqUpdate = "
                                UPDATE	
                                    par.subacaoitenscomposicao
                                SET
                                    icoquantidaderecebida = {$valor}
                                    ,icousucpfmonitoramento = '{$_SESSION['usucpf']}'
                                    ,icodatamonitoramento = 'now()'
                                    $ativa
                                WHERE icoid = {$v['id']}";

                            // Executa a query
                            $db->executar($sqUpdate);	
			}
		}
	}
	// Executa as queries
	$db->commit();
	
	// Caso contenha erro exibe mensagem
	if( $contemErro )
	{
		$strItens = implode(',', $itensComErro);
		if(count( $itensComErro) > 1 )
		{
			die( 'Alguns itens não puderam ser salvos pois a quantidade detalhada é maior do que a informada como recebida, são eles:' .
			$strItens.
			" Os demais itens foram salvos com sucesso."
			);
		}
		else
		{
			die( 'O item ' . $strItens
			. ' não  pode ser salvo porque a quantidade detalhada é maior do que a informada como recebida. Os demais itens foram salvos com sucesso.'
			);
		}
	}
	else
	{
		die( 'Todos os itens foram atualizados com Sucesso!');
	}
}

// Caso seja a requisição ajax redireciona para a função carregaDadoItens
if( $_REQUEST['requisicao'] == 'SalvarDetalhamentoItem' )
{
	ob_clean();
	// Recebe uma string compactada com os resultados
	$strParams = $_REQUEST['arrParams'];
	
	// Retira o "|" do final para transformar em array os valores dos campos
	$strParams = substr_replace($strParams, '', -1);
	
	// Cria um array com os valores dos campos
	$arrParams = explode('|', $strParams);
	// Inicia array com os itens
	$arrItensMonitoramento = array();
	
	// Cria o array com os códigos e com os valores
	foreach ( $arrParams as $k => $v )
	{
		$arrTemp = explode(',', $v);
		
		$arrItensMonitoramento[$k]['id'] = $arrTemp[0];
		$arrItensMonitoramento[$k]['valor'] = $arrTemp[1];
		
	}
	// Adiciona os novos valores caso não exista, se existirem ele atualiza os dados.
	foreach( $arrItensMonitoramento as $k => $v )
	{
		// Caso o valor tenha sido deixado em branco ele coloca o valor de 0 para o insert/update 
		$valor = ( $v['valor'] ) ? $v['valor'] : 0;
		
		// Atualiza as informações do item na tabela par.subacaoitenscomposicao
		$sqUpdate = "
									
			UPDATE	
				par.subacaoitenscomposicao
			SET
				icoquantidadevistoriadainmetro	= {$valor}
			WHERE
				icoid 			= {$v['id']}
		";
		
		// Executa a query
		$db->executar($sqUpdate);
		
	}
	// Executa as queries
	$db->commit();

	exit();
}
// Função que retorna os dados dos itens
function carregaDadoItensMonitoramento()
{
	global $db;
	
	$dopid = $_POST['dopid'];
	if( $_POST['habil'] == 'S' && (! bloqueiaParaConsultaEstMun()) && ( getFinalizado( $dopid ) != 'finalizado' ) )
	{
		$btnRepetir = "CASE WHEN
							si.icoquantidaderecebida > 0
						THEN
							CASE WHEN si.icoquantidaderecebida = 0 THEN	
								'<input '|| CASE WHEN s.sbacronograma = 2 
												THEN 'onclick=\"abreEscolas('||s.sbaid||','|| si.icoano||','|| si.icoid||', \\'tec\\' )\"' 
												ELSE '' 
											END || ' type=\"text\"  id=\"' || si.icoid || '\" onKeyUp=\"this.value=mascaraglobal('|| '\'#########\'' || ',this.value);\"   value=\"\">'
							ELSE
								'<input '|| CASE WHEN s.sbacronograma = 2 THEN 'onclick=\"abreEscolas('||s.sbaid||','|| si.icoano||','|| si.icoid||', \\'tec\\')\"' ELSE ''END || ' type=". '"' ."text". '"' . "  id=". '"' ."' || si.icoid || '". '"' .  " onKeyUp=". '"' ."this.value=mascaraglobal('|| '\'#########\'' || ',this.value);". '"' .  "   value=". '"' ."' ||  si.icoquantidaderecebida || '". '"' .  ">'
							END
						ELSE
							CASE WHEN si.icomonitoramentocancelado = TRUE
								THEN
									'<input '|| CASE WHEN s.sbacronograma = 2 
													THEN 'onclick=\"abreEscolas('||s.sbaid||','|| si.icoano||','|| si.icoid||',\\'tec\\'); \"' 
													ELSE '' 
												END || ' type=\"text\"  id=\"' || si.icoid || '\" onKeyUp=\"this.value=mascaraglobal('|| '\'#########\'' || ',this.value);\"   
										value=\"' ||  si.icoquantidaderecebida || '\">'
								ELSE
									'<input '|| CASE WHEN s.sbacronograma = 2 
													THEN 'onclick=\"abreEscolas('||s.sbaid||','|| si.icoano||','|| si.icoid||', \\'tec\\'); \"' 
													ELSE '' 
												END || ' type=\"text\"  id=\"' || si.icoid || '\" onKeyUp=\"this.value=mascaraglobal('|| '\'#########\'' || ',this.value);\"   value=\"\">'
								END
						END 
						|| '&nbsp;' ||
						CASE WHEN s.sbacronograma <> 2 
							THEN '<input type=\"button\"  value=\"' || 'Bens/Serviços conforme termo' || '\" 
										onclick=\"repeteQtd(' || 
											(CASE WHEN sbacronograma = 1 
												THEN ( 
													SELECT DISTINCT
		                                               	CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
						                           	FROM par.subacaoitenscomposicao sic 
						                         	WHERE sic.sbaid = s.sbaid
						                          		AND sic.icoano = sd.sbdano
						                           		AND sic.icoid = si.icoid
                                                                                        AND sic.icostatus = 'A'
					                         		GROUP BY sic.sbaid, sic.icovalidatecnico )
		               							ELSE ( 
													SELECT DISTINCT
		                                            	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
		                                                	THEN -- escolas sem itens
		                                                    	sum(coalesce(se.sesquantidadetecnico,0))
		                                                   	ELSE -- escolas com itens
		                                                    	CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
		                                                        	sum(coalesce(ssi.seiqtdtecnico,0))
		                                                       	END
		                                            	END as vlrsubacao
		                                       		FROM entidade.entidade t
		                                            INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid
		                                            LEFT  JOIN entidade.entidadedetalhe 			ed  ON t.entid = ed.entid
		                                            INNER JOIN entidade.endereco 					d   ON t.entid = d.entid
		                                            LEFT  JOIN territorios.municipio 				m   ON m.muncod = d.muncod
		                                            LEFT  JOIN par.escolas 							e   ON e.entid = t.entid
		                                            INNER JOIN par.subacaoescolas 					se  ON se.escid = e.escid
		                                            INNER JOIN par.subacaoitenscomposicao 			sic ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
		                                            LEFT  JOIN  par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
		                                            WHERE 
														sic.sbaid = s.sbaid 
														AND sic.icoano = sd.sbdano
		                                            	AND sic.icoid = si.icoid
		                                               	AND (t.entescolanova = false OR t.entescolanova is null) 
														AND t.entstatus = 'A' 
														AND f.funid = 3 
                        								AND ( CASE WHEN inu.itrid = 2 THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
		                                            GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
		                END )
			|| ',' || si.icoid ||  ')' ||  '\">'
		ELSE
			''
		END
		"
		;
	}
	else
	{
		$btnRepetir = "
				CASE WHEN si.icoquantidaderecebida > 0
					THEN
						CASE WHEN si.icoquantidaderecebida = 0 
		                	THEN ''
							ELSE '' || si.icoquantidaderecebida || ''
						END
					ELSE
						CASE WHEN 
								si.icomonitoramentocancelado = TRUE
							THEN
								'' || si.icoquantidaderecebida || ''
							ELSE
								''
						END
				END";
	}
	
	$sql = "SELECT DISTINCT
				si.icodescricao,
				(CASE WHEN sbacronograma = 1 
					THEN ( 
	                	SELECT DISTINCT
	                    	CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
	                    FROM par.subacaoitenscomposicao sic 
	                    WHERE 
	                    	sic.sbaid = s.sbaid
	                    	AND sic.icoano = sd.sbdano
	                    	AND sic.icoid = si.icoid
                                AND sic.icostatus = 'A'
	                    GROUP BY sic.sbaid, sic.icovalidatecnico 
						)
			     	ELSE ( 
			     		SELECT DISTINCT
                        	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
                            	THEN -- escolas sem itens
                                	sum(coalesce(se.sesquantidadetecnico,0))
                                ELSE -- escolas com itens
                                	CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
                                    	sum(coalesce(ssi.seiqtdtecnico,0))
                                    END
                            END as vlrsubacao
	                   	FROM entidade.entidade t
	                    inner join entidade.funcaoentidade f on f.entid = t.entid
	                    left join entidade.entidadedetalhe ed on t.entid = ed.entid
	                    inner join entidade.endereco d on t.entid = d.entid
	                    left join territorios.municipio m on m.muncod = d.muncod
	                    left join par.escolas e on e.entid = t.entid
	                    INNER JOIN par.subacaoescolas se ON se.escid = e.escid
	                    INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
	                    LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
	                    WHERE 
	                    	sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
		                    AND sic.icoid = si.icoid
		                    AND (t.entescolanova = false OR t.entescolanova is null) AND t.entstatus = 'A' AND f.funid = 3 
                        	AND ( CASE WHEN inu.itrid = 2 THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
	                    GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
                END ) as quantidade,
				CASE WHEN dps.dpsid IS NOT NULL 
					THEN '<img src=../imagens/atencao.png style=cursor:pointer; onclick=\"alert(''Este item se encontra em reprogramação.'');\" >'
					ELSE {$btnRepetir}
				END as repetir
			FROM par.subacao s
			/*Para recuperar o itrid - ver se é estadual ou municipal*/
			INNER JOIN par.acao 				aca on aca.aciid = s.aciid AND acistatus = 'A'
			INNER JOIN par.pontuacao 			pon on pon.ptoid = aca.ptoid AND ptostatus = 'A'
			INNER JOIN par.instrumentounidade 		inu on inu.inuid = pon.inuid
			/*FIM - Para recuperar o itrid*/
			INNER JOIN par.subacaodetalhe 					sd   ON sd.sbaid = s.sbaid
			LEFT  JOIN par.documentoparreprogramacaosubacao dps
				INNER JOIN par.reprogramacao 			rep ON rep.repid = dps.repid AND rep.repstatus = 'A'
				INNER JOIN par.vm_documentopar_ativos 	dp  ON dp.dopid = dps.dopid
				ON dps.sbdid = sd.sbdid AND dps.dpsstatus = 'A'
			INNER JOIN par.termocomposicao 					tc   ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao 			si   ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
			INNER JOIN par.subacaoitenscomposicaocontratos 	sicc ON si.icoid = sicc.icoid
			INNER JOIN par.propostaitemcomposicao 			pic  ON pic.picid = si.picid
			LEFT  JOIN par.empenhopregaoitensenviados 		emi  ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
			WHERE 
				sd.sbdid = {$_POST['sbdid']}
				AND icomonitoramentocancelado = false
			ORDER BY si.icodescricao";
//	ver($sql, d);
	$cabecalho = array("Descrição do Item", "Quantidade do Termo", "Quantidade recebida" );
	$db->monta_lista($sql,$cabecalho,5000,5,'N','90%','N');
}

// Função que retorna os dados dos itens
function carregaItensAnaliseFNDE()
{
    $dopid = $_POST['dopid'];
	global $db;

	$sql = "SELECT  DISTINCT
				CASE WHEN ( SELECT dps.dpsid FROM par.documentoparreprogramacaosubacao dps
							INNER JOIN par.reprogramacao 			rep ON rep.repid = dps.repid AND rep.repstatus = 'A'
							INNER JOIN par.vm_documentopar_ativos 	dp  ON dp.dopid = dps.dopid
							WHERE dps.sbdid = sd.sbdid AND dps.dpsstatus = 'A' ) IS NOT NULL 
					THEN '<img src=../imagens/atencao.png style=cursor:pointer; onclick=\"alert(''Este item se encontra em reprogramação.'');\" >'
					ELSE '<center><input type=checkbox name=icoids[] value='|| si.icoid ||' /></center>'
				END as chk,
				si.icodescricao,
				( CASE 
					WHEN sbacronograma = 1 THEN ( 	SELECT DISTINCT
														CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
													FROM 
														par.subacaoitenscomposicao sic
													WHERE 
														sic.sbaid = s.sbaid
														AND sic.icoano = sd.sbdano
														and sic.icoid = si.icoid
                                                                                                                AND sic.icostatus = 'A'
													GROUP BY sic.sbaid, sic.icovalidatecnico )
					ELSE (	SELECT DISTINCT
								CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN sum(coalesce(se.sesquantidadetecnico,0))-- escolas sem itens
								ELSE CASE 
										WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(ssi.seiqtdtecnico,0)) -- validado (caso não o item não é contado) -- escolas com itens
									END
								END as vlrsubacao
							FROM 
								entidade.entidade t
							INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid
							LEFT  JOIN entidade.entidadedetalhe 			ed  ON t.entid = ed.entid
							INNER JOIN entidade.endereco 					d   ON t.entid = d.entid
							LEFT  JOIN territorios.municipio 				m   ON m.muncod = d.muncod
							LEFT  JOIN par.escolas 							e   ON e.entid = t.entid
							INNER JOIN par.subacaoescolas 					se  ON se.escid = e.escid
							INNER JOIN par.subacaoitenscomposicao 			sic ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
							LEFT  JOIN par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
							WHERE 
								sic.sbaid = s.sbaid 
								AND sic.icoano = sd.sbdano
								AND sic.icoid = si.icoid
								AND (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
                        		AND ( CASE WHEN inu.itrid = 2 THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
							GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
				END ) as quantidade,
				'<div style=\"width:48%; float:left; text-align: center\">' || si.icoquantidaderecebida || '</div><div style=\"width:48%; float:left; text-align: center\"><img src=\"../imagens/schedule.png\" title=\"Histórico de Acompanhamento: Monitoramento\" style=\"cursor:pointer\" onclick=\"historicoAcompanhamento('||si.icoid||', \'monitoramento\')\"></div>' as qtd_recebida,
				CASE WHEN
					EXISTS(SELECT 
							sit.icoid
						FROM
							par.subacaoitenscomposicaoContratos sicon 
						INNER JOIN par.subacaoitenscomposicao sit ON sit.icoid = sicon.icoid AND sit.icostatus = 'A'
						WHERE sicon.icoid = si.icoid )
					THEN '<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"listaDownloadAnexo('|| si.icoid ||', \'{$dopid}\' );\" > &nbsp; <img onclick=\"historicoAcompanhamento('|| si.icoid ||', \'contratos\')\" style=\"cursor:pointer\" title=\"Histórico de Acompanhamento: Contratos\" src=\"../imagens/schedule.png\"></center>' 
					ELSE '<center><img onclick=\"historicoAcompanhamento('|| si.icoid ||', \'contratos\')\" style=\"cursor:pointer\" title=\"Histórico de Acompanhamento: Contratos\" src=\"../imagens/schedule.png\"></center>'
				END as contratos_item,
				CASE WHEN
					EXISTS(SELECT 
								scn.icoid
							FROM
								par.subacaoitenscomposicaonotasfiscais scn 
							INNER JOIN par.subacaoitenscomposicao sit ON sit.icoid = scn.icoid AND sit.icostatus = 'A'
							WHERE scn.icoid = si.icoid )
					THEN '<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"listaDownloadNotas('|| si.icoid ||', {$dopid} );\" /> &nbsp; <img onclick=\"historicoAcompanhamento('|| si.icoid ||', \'notas\')\" style=\"cursor:pointer\" title=\"Histórico de Acompanhamento: Notas\" src=\"../imagens/schedule.png\"></center>' 
					ELSE '<center><img onclick=\"historicoAcompanhamento('|| si.icoid ||', \'notas\')\" style=\"cursor:pointer\" title=\"Histórico de Acompanhamento: Notas\" src=\"../imagens/schedule.png\"></center>'
				END as notas_item,
				CASE WHEN EXISTS(SELECT sap.icoid FROM par.subacaoitenscomposicao_acompanhamentoparecer sap WHERE sap.icoid = si.icoid )
					THEN '<center><img src=../imagens/icone_lupa.png style=cursor:pointer; icoid='|| si.icoid ||' class=listaParecer /></center>'
					ELSE ''
				END as parecer_item
			FROM 
				par.subacao s
			/*Para recuperar o itrid - ver se é estadual ou municipal*/
			INNER JOIN par.acao 				aca on aca.aciid = s.aciid AND acistatus = 'A'
			INNER JOIN par.pontuacao 			pon on pon.ptoid = aca.ptoid AND ptostatus = 'A'
			INNER JOIN par.instrumentounidade 		inu on inu.inuid = pon.inuid
			/*FIM - Para recuperar o itrid*/
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
			INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
			LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
			WHERE 
				sd.sbdid = {$_POST['sbdid']}
				AND icomonitoramentocancelado = false
			ORDER BY si.icodescricao";
	$cabecalho = array("&nbsp;", "Descrição do Item", "Quantidade do Termo", "Quantidade recebida", "Contratos Anexados", "Notas Fiscais Anexadas", "Histórico de Parecer" );
	$db->monta_lista($sql,$cabecalho,5000,5,'N','90%','N');
}

// Função que retorna os dados dos itens
function carregaDadoItens()
{
	global $db;
	
	$dopid = $_POST['dopid'];
	if( $_POST['habil'] == 'S' && (! bloqueiaParaConsultaEstMun() ) && (  getFinalizado($dopid) != "finalizado" ) )
	{
		$check = "CASE WHEN dps.dpsid IS NOT NULL 
						THEN '<img src=../imagens/atencao.png style=cursor:pointer; onclick=\"alert(''Este item se encontra em reprogramação.'');\" >'
						ELSE '<input type=". '"' ."checkbox". '"' . "  id=". '"' ."' || si.icoid || '". '"' .  "  value=". '"' ."' || si.icoid || '". '"' .  ">'
					END,";
		$btnCancela =  "
                        CASE WHEN
				icomonitoramentocancelado = false
			THEN
				'<input type=". '"' ."button". '"' .  "  value=". '"' ."' || 'Cancelar Item' || '". '"' .  " onclick=\" cancelaMonitoramentoItem(' || si.icoid ||  ') \">' 
			ELSE
				'<input type=". '"' ."button". '"' .  "  value=". '"' ."' || 'Reativar Item' || '". '"' .  " onclick=\" ativaMonitoramentoItem(' || si.icoid ||  ') \">'
			END
				as cancelar";
	}
	else
	{
		$check = "'<input type=". '"' ."hidden". '"' . "  id=". '"' ."' || si.icoid || '". '"' .  "  value=". '"' ."' || si.icoid || '". '"' .  ">',";
		$btnCancela =  "'' as cancelar";		
	}
	
	$sql = "SELECT  DISTINCT
				{$check}
				si.icodescricao,
				(CASE WHEN sbacronograma = 1 THEN
	                	( SELECT DISTINCT CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
		                FROM par.subacaoitenscomposicao sic 
		                WHERE 
		                	sic.sbaid = s.sbaid
		                	AND sic.icoano = sd.sbdano
		                	AND sic.icoid = si.icoid
                                        AND sic.icostatus = 'A'
		                GROUP BY sic.sbaid, sic.icovalidatecnico )
                	ELSE 
                    	( SELECT DISTINCT
                        	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
                            	THEN -- escolas sem itens
                                	sum(coalesce(se.sesquantidadetecnico,0))
                                ELSE -- escolas com itens
                                	CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(ssi.seiqtdtecnico,0)) END -- validado (caso não o item não é contado) 
                            END as vlrsubacao
                  		FROM entidade.entidade t
                     	INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid
                        LEFT  JOIN entidade.entidadedetalhe 			ed  ON t.entid = ed.entid
                        INNER JOIN entidade.endereco 					d   ON t.entid = d.entid
                        LEFT  JOIN territorios.municipio 				m   ON m.muncod = d.muncod
                        LEFT  JOIN par.escolas 							e   ON e.entid = t.entid
                        INNER JOIN par.subacaoescolas 					se  ON se.escid = e.escid
                        INNER JOIN par.subacaoitenscomposicao 			sic ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
                        LEFT  JOIN  par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
                        WHERE 
                        	sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
                        	AND sic.icoid = si.icoid
                        	AND (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' AND f.funid = 3 
                        	AND ( CASE WHEN inu.itrid = '2' THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
                        GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
                END ) as quantidade,
				TO_CHAR( si.icovalor, '999G999G999D99'),
				TO_CHAR( ( si.icoquantidadetecnico * si.icovalor), '999G999G999D99') as valortotal,
				emi.adesao,
                                (
                                    array_to_string(array(
                                        SELECT distinct
                                            CASE WHEN (con.condiligencia) IS NOT NULL 
                                                THEN '<span style=\"color: #FF0000\">' || con.connumerocontrato || '</span>'
                                                ELSE '<span>' || con.connumerocontrato || '</span>'
                                            END
                                        FROM par.subacaoitenscomposicaocontratos sicc
                                        INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicc.icoid AND sic.icostatus = 'A'
                                        INNER JOIN par.contratos con ON con.conid=sicc.conid AND con.constatus = 'A'
                                        WHERE sic.sbaid = s.sbaid AND sic.icoid = si.icoid AND sicc.sccstatus = 'A'), ', ')
                                ) as contratos,
				CASE WHEN
					EXISTS(	
						SELECT 
							sit.icoid
						FROM
							
						par.subacaoitenscomposicaoContratos sicon 
						INNER JOIN par.subacaoitenscomposicao sit ON si.icoid = sicon.icoid AND sit.icostatus = 'A'
					
						where sicon.icoid = si.icoid
					)
				THEN
					'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"listaDownloadAnexo('|| si.icoid ||', \'{$dopid}\' );\" >' 
				ELSE
					''
				END as contratos_item,
				{$btnCancela}																																					  
			FROM par.subacao s
			/*Para recuperar o itrid - ver se é estadual ou municipal*/
			INNER JOIN par.acao 				aca on aca.aciid = s.aciid AND acistatus = 'A'
			INNER JOIN par.pontuacao 			pon on pon.ptoid = aca.ptoid AND ptostatus = 'A'
			INNER JOIN par.instrumentounidade 	inu on inu.inuid = pon.inuid
			/*FIM - Para recuperar o itrid*/
			INNER JOIN par.subacaodetalhe 					sd  ON sd.sbaid = s.sbaid
			LEFT JOIN par.documentoparreprogramacaosubacao 	dps
				INNER JOIN par.reprogramacao 			rep ON rep.repid = dps.repid AND rep.repstatus = 'A'
				INNER JOIN par.vm_documentopar_ativos 	dp  ON dp.dopid = dps.dopid
																ON dps.sbdid = sd.sbdid AND dps.dpsstatus = 'A'
			INNER JOIN par.termocomposicao 					tc  ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao 			si  ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
			INNER JOIN par.propostaitemcomposicao 			pic ON pic.picid = si.picid
			LEFT  JOIN par.empenhopregaoitensenviados 		emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.prpid = {$_POST['prpid']}  AND emi.epistatus = 'A'
			WHERE 
				sd.sbdid = {$_POST['sbdid']}
				AND si.icovalidatecnico = 'S' 
			ORDER BY
				si.icodescricao";
	$cabecalho = array("Ação", "Descrição do Item", "Quantidade", "Valor", "Valor Total", 'Número da adesão', 'Contratos', 'Contratos do Item', 'Cancelar Item' );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
}

// Caso não tenha o id do documento deve redirecionar para a página anterior
if( (! $dopid) || ($dopid == '') || (empty($dopid)) )
{	
	$dopid = $_REQUEST['dopid'];
	
	if( (! $dopid) || ($dopid == '') || (empty($dopid)) )
	{
	
		if( ! $_SESSION['dopid'] )
		{
			echo '<script>
						alert("Faltam dados.");
						window.location="par.php?modulo=principal/administracaoDocumentos&acao=A";
				</script>';
			die;
		
		}
		else
		{
			$dopid = $_SESSION['dopid'];
		}
	}
}

 

function temPagamento($dopid)
{
	global $db;
	// Verifica se existe pagamento
	$sql = 
		"
			SELECT
			      id	
			AS ac		            
				 from (
				SELECT 	
					dp.dopid as id,					
					(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, 
					dp.dopvalortermo::numeric(20,2), 
					em.valor as valorempenho, 
					pm.valor_pagamento as valorpagamento,
					dp.arqid,
					dp.dopdatafimvigencia
				FROM 
					par.documentopar  dp
				INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
				INNER JOIN par.processopar pp ON pp.prpid = dp.prpid AND pp.prpstatus = 'A'
				INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
				INNER JOIN ( select 
								d.dopid, 
								sum(vve.vrlempenhocancelado) + sum(coalesce(emr.vrlreforco,0)) as valor
							from 
								par.documentopar d
							inner join par.processopar prp on prp.prpid = d.prpid AND prp.prpstatus = 'A'
							inner join par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
							inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
							left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
										from par.empenho
										where empcodigoespecie in ('02') and empstatus = 'A'
										group by 
											empnumeroprocesso,
											empcodigoespecie,
											empidpai) as emr on emr.empidpai = emp.empid 
							inner join par.empenhosubacao ems on ems.empid = emp.empid AND eobstatus = 'A' group by d.dopid ) em ON em.dopid = dp.dopid
				LEFT JOIN (select 
								d.dopid, sum( pobvalorpagamento ) as valor_pagamento
							from 
								par.documentopar d
							inner join par.processopar prp on prp.prpid = d.prpid  and prp.prpstatus = 'A'
							inner join par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
							inner join par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
							inner join par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' group by d.dopid) pm ON pm.dopid = dp.dopid
			) as foo
			WHERE
				id = {$dopid}
				/* -- Regra retirada por solicitação do Júlio em 12/03/2014
				-- AND valorpagamento > 0
				*/ 
				
		";
	
	$result = $db->carregar($sql);

	if( (count($result)) && (is_array($result)) )
	{
		return true;
		
	}
	else
	{
		return false;
	}
	
}

/**
 * @desc: Funcão que irá retornar qual o tipo do acompanhamento, que pode ser SOMENTE ONIBUS, MISTO ( ONIBUS E OUTRa subacao ) OU SEM ÔNIBUS  
 * @return int
 * 		1 - Somente 
 * 		2 - Onibus e outro tipo
 * 		3 - Sem onibus
 * @author elias.oliveira
 * */
function perfilTipoAcompanhamento($dopid)
{
	global $db;
	
	// Verifica se as ações são onibus/mista/semonibus
	$sql2 = "SELECT 					
			CASE WHEN 	( COUNT( foo.sbaid ) = COUNT(foo.onibus) ) AND ( COUNT( foo.sbaid ) > 0 ) THEN -- somente onibus
				1
			WHEN 		( COUNT( foo.sbaid ) > COUNT(foo.onibus) ) AND ( COUNT( foo.sbaid ) > 0 ) AND (COUNT(foo.onibus) > 0) THEN
				2	
			ELSE
				3
			END
			as onibus
		from (
			select   
				( SELECT  s1.sbaid  FROM par.subacao s1 WHERE s1.sbaid = s.sbaid and s1.prgid in (81,50,153,164,158)) as onibus,
				 s.sbaid
			from par.documentopar  dp
			inner join  par.termocomposicao tc on tc.dopid = dp.dopid
			inner join par.subacaodetalhe sd on sd.sbdid = tc.sbdid
			inner join par.subacao s on s.sbaid = sd.sbaid
			WHERE dp.dopid = {$dopid}
		) as foo";
	
	$result2 				= $db->carregar($sql2);
	$nTipoAcompanhamento	= $result2[0]['onibus'];
	return $nTipoAcompanhamento;
	
}
 $perfilTipoAcompanhamento = perfilTipoAcompanhamento($_REQUEST['dopid']);

if($_REQUEST['reabrirAcompanhamento'])
{
	
	if (getFinalizado() == 'finalizado')
	{
		if( in_array( PAR_PERFIL_EQUIPE_FINANCEIRA , $perfil ) ||  in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array(  PAR_PERFIL_SUPER_USUARIO, $perfil ) )
		{
			$sql = "update par.documentopar set dopacompanhamento = true where dopid = " . $dopid;
			$db->executar($sql);
			$db->commit();

			echo '<script>alert("O acompanhamento do termo foi aberto para edição novamente.")</script>';
		}
	}
}
 
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/par.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
function listaDownloadAnexo( icoid, dopid )
{
	window.open('par.php?modulo=principal/popupDownloadAcompanhamento&acao=A&icoid=' + icoid + "&dopid=" + dopid,  '' ,'width=780,height=300,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);  

}

function repeteQtd(qtd, item)
{
	// pega item e atribui valor da sua quantidade 
	$('#' + item).val(qtd);
}

function carregaTermoMinuta(dopid){
	window.open('par.php?modulo=principal/visualizaTermoCompromisso&acao=A&dopid='+dopid,'visualizatermo','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

function reabreAcompanhamento( dopid )
{

	if(confirm("Deseja realmente abrir novamente o acompanhamento para este termo?"))
	{
		window.location.href = 'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&dopid='+dopid+"&reabrirAcompanhamento=true";
	}
}

function listaDownloadNotas( icoid, dopid )
{
	window.open('par.php?modulo=principal/popupDownloadAcompanhamentoNotas&acao=A&icoid=' + icoid + '&dopid=' + dopid,  '' ,'width=780,height=300,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);  
}

function historicoAcompanhamento(id, tipoacompanhamento){
    return window.open('par.php?modulo=principal/popupHistoricoAcompanhamento&acao=A&id='+id+'&tipoacompanhamento='+tipoacompanhamento, 
       'Histórico de Acompanhamento: Contratos', 
       "height=400,width=900,scrollbars=yes,top=50,left=200" ).focus();	
}
</script>
<?php

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$url =  'par.php?modulo=principal/acompanhamentoDocumentos&acao=A';

$db->cria_aba( $abacod_tela, $url, '');

?>  
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td>
		<?php 
		if( ( getFinalizado() == "finalizado" ) )
		{
			if( in_array( PAR_PERFIL_EQUIPE_FINANCEIRA , $perfil ) ||  in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array(  PAR_PERFIL_SUPER_USUARIO, $perfil ) )
			{
				echo"<center><input type=\"button\" title=\"Reabrir acompanhamento Finalizado\" value=\"Reabrir acompanhamento\" onclick=\"reabreAcompanhamento('{$dopid}' )\" ></center>";											
			} 
		}
		
		$_REQUEST['abas'] = (empty($_REQUEST['abas']) ? 'acompanhamentoContratos' : $_REQUEST['abas']);
		
		include_once APPRAIZ . "par/modulos/principal/{$_REQUEST['abas']}.inc";
		?>
		</td>
	</tr>
</table>