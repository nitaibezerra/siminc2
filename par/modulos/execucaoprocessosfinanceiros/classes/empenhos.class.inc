<?php
class Empenho extends Processos{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "execucaofinanceira.empenho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "empid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'empid' => null, 
									  	'empnumeroprocesso' => null, 
									  	'empcnpj' => null, 
									  	'empnumeroempenhosigef' => null, 
									  );

	/*								  
	function __construct($controleMontagemDeTela = NULL) {
		
	}
		*/	

	function __construct() {
		parent::__construct();
	}
									  								  
	public function listaprocessos($post = NULL){
		$cabecalho = Array('Ação','Numero do Processo','Nº no sistema Documenta','UF','Código IBGE', 'Município', 'Data de Inclusão','Processo do Tipo');
		
		$where = array();
		if($post['filtroGeral']){
			$filtros = $post['filtroGeral']; 
			$where[]= " ( 	
							removeacento(p.pronumeroprocesso) ilike removeacento(('%{$filtros}%'))
							OR removeacento(lower(p.prodocumenta)) ilike ('%{$filtros}%')
							OR removeacento(lower(p.estuf)) ilike ('%{$filtros}%')
							OR removeacento(p.muncod) ilike ('%{$filtros}%')
							OR removeacento(lower(m.mundescricao)) ilike removeacento(('%{$filtros}%'))
						)	
					";
		}
		
		if($post['requisicao'] == 'pesquisar') {		
			if($post['numeroprocesso']) 		$where[] = "p.pronumeroprocesso = '".$post['numeroprocesso']."'";
			if($post['municipio']) 				$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$post['municipio']."%')";
			if($post['estuf']) 					$where[] = "p.estuf='".$post['estuf']."'";
			if($post['empenhado'] == "1") 		$where[] = " p.pronumeroprocesso in (select distinct empnumeroprocesso from par.empenho where empnumeroprocesso = p.pronumeroprocesso) ";  //"(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) > 0";
			if($post['empenhado'] == "2") 		$where[] = " p.pronumeroprocesso not in (select distinct empnumeroprocesso from par.empenho where empnumeroprocesso = p.pronumeroprocesso) "; // "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) = 0";
			if($post['tipoExecucao'] == "C")	$where[] = "p.protipoexecucao = 'C'";
			if($post['tipoExecucao'] == "T") 	$where[] = "p.protipoexecucao = 'T'";
			
			if($post['esfera'] == "e") {
				$where[] = "p.proesfera = 'E' ";
			} 
			if($post['esfera'] == "m") {
				$where[] = "p.proesfera = 'M' ";
			}
	
			if($post['anoprocesso']) $where[] = "p.proano = '".$post['anoprocesso']."'";
		}
		
		$sql = "SELECT 	'<center> 
							<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"visualizarProcesso('|| p.proid ||')\"><img title=\"Visualizar todo o processo.\" style=\"width: 18px; height: 18px;\" src=\"/imagens/livro.gif\" border=0 title=\"Selecionar\"></a> 
							<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreProcessoEmpenho('|| p.proid ||','|| CASE WHEN e.empid IS NOT NULL THEN e.empid ELSE 0 END ||')\"><img title=\"Alterar dados do processo.\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
							<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirProcesso('|| p.proid ||')\"><img title=\"Excluir processo.\" src=\"/imagens/excluir.gif\" border=0 title=\"Selecionar\"></a>
						</center>' as acao, 
						p.pronumeroprocesso, p.prodocumenta, p.estuf, p.muncod, m.mundescricao, to_char(p.prodatainclusao, 'DD/MM/YYYY') as data, tp.tipdescricao
				FROM execucaofinanceira.processos p
					inner join execucaofinanceira.tipoprocesso tp on tp.tipid = p.tipid
					LEFT JOIN execucaofinanceira.empenho e ON e.empnumeroprocesso = p.pronumeroprocesso
					LEFT JOIN territorios.municipio m ON m.muncod = p.muncod
				WHERE 1=1 ".($where ? " and ".implode(" and ", $where) : '');

		return Processos::monta_lista( $sql, $cabecalho, 50, 20, 'N', 'center','N');

	}
	
	
	public function listaDadosPassiveisDeEmpenho($obProcesso){
		// Mudar o TCPID
		if($obProcesso->tipid == parent::SUBACOES_GENERICO_PAR){
			$titulo = "Lista de Subações a serem Empenhadas";
			$sql = $this->sqlListaDeSubacoesAEmpenhar($obProcesso);
		}else if($obProcesso->tipoExecucao == 2){
		
		}
		$cabecalho = array("Selecione", "Nº do Empenho", "Subação", "Ano", "Valor da Subação", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar" );
				
		echo '<table class="tabela" id="dadosASeremEmpenhados" align="center" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<thead>
					<tr>
						<td class="subtituloCentro">'.$titulo.'</td>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td><div style="overflow-x: auto; overflow-y: auto; width:100%; height:300px;">';
		
							$this->monta_lista_simples($sql,$cabecalho,500,5,'S','99.5%','S');
		echo '			</div></td>
				</tr>
			</tbody>
		</table>
		';	
		return true;
	}

	

	public function listaDados100ProcentoEmpenhadas($obProcesso){
		
		if($obProcesso->tipid == parent::SUBACOES_GENERICO_PAR){
			$titulo = "Lista de Subações 100% empenhadas do tipo Termo e 99% do tipo Convênio";
			$sql = $this->sqlListaDeSubacoes100Empenhado($obProcesso);
		}else if($obProcesso->tipoExecucao == 2){
		
		}
			
		echo '<table class="tabela" id="dadosASeremEmpenhados" align="center" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<thead>
					<tr>
						<td class="subtituloCentro">'.$titulo.'</td>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>';
				$this->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
		echo '			</td>
				</tr>
			</tbody>
		</table>';	
		return true;
	}

	// ABAIXO FICA FUNCOES DE QUE MONTA OS SQL'S
	public function sqlListaDeSubacoesAEmpenhar($obProcesso){
		
		if( $obProcesso->esfera == 'M' ){
			$joinprocesso = " INNER JOIN execucaofinanceira.processos	p  ON p.muncod = iu.muncod ";
		} else {
			$joinprocesso = " INNER JOIN execucaofinanceira.processos p on p.estuf = iu.estuf AND p.muncod is null ";
		}
		
		$sql = "SELECT DISTINCT dados.chk || '<a href=\"javascript:listarSubacao(\'' || dados.sbaid || '\', \'' || dados.sbdano || '\', \'' || dados.inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Subação\" border=\"0\"></a>' as acoes,
					dados.numeroempenho,
					dados.descricaosubacao,
					dados.sbdano || '&nbsp;' as ano,
					dados.valorsubacao,
					dados.percentualempenhado,
					dados.valorempenhado,
					dados.porcentagemaempenhar,
					'<input type=text style=\"text-align:right;\" onKeyUp=\"calculaEmpenho(this);\" class=normal id=id_vlr_'||dados.sbdid||' value='||
										CASE WHEN dados.percentualempenhado <> 0
											THEN to_char( (dados.valorsubacao -  dados.valorempenhado), '999G999G999D99' )
											ELSE to_char( (((dados.valorsubacao * dados.porcentagem)::numeric / 100)::numeric), '999G999G999D99')
										END
							||' name=name_vlr_'||dados.sbdid
					AS valoraempenhar,
					CASE WHEN dados.percentualempenhado <> 0
						THEN '<input type=hidden id=vlrTotal_'||dados.sbdid||' name=vlrTotal_'||dados.sbdid||' value='|| to_char((dados.valorsubacao -  dados.valorempenhado)::numeric, '999G999G999D99') ||'>'
						ELSE '<input type=hidden id=vlrTotal_'||dados.sbdid||' name=vlrTotal_'||dados.sbdid||' value='|| to_char((dados.valorsubacao * dados.porcentagem)::numeric / 100::numeric, '999G999G999D99') ||'>'
					END AS valoraempenharT
			FROM (
					SELECT '<input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||sd.sbdid||' value='||sd.sbdid||'>' as chk, 
							s.sbaid,
							e.empnumeroempenhosigef as numeroempenho, 
							s.sbadsc as descricaosubacao, 
							CASE WHEN sbacronograma = 1 
								THEN ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
											CASE WHEN sic.icovalidatecnico IS NULL -- antigos
												THEN sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
												ELSE -- novos
													CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
														sum(coalesce(sic.icoquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
													END
												END
											as vlrsubacao
									FROM par.subacaoitenscomposicao sic 
									WHERE sic.sbaid = s.sbaid
									AND sic.icoano = sd.sbdano
									GROUP BY sic.sbaid, sic.icovalidatecnico ) as foo )
								ELSE ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
										CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
											THEN -- escolas sem itens
												CASE WHEN se.sesvalidatecnico IS NULL -- antigos
												THEN 
													sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
												ELSE -- novos
													sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
												END
											ELSE -- escolas com itens
												CASE WHEN sic.icovalidatecnico IS NULL -- antigos
												THEN
													sum(coalesce(ssi.seiqtd,0) * coalesce(sic.icovalor,0))::numeric(20,2)
												ELSE -- novos
													CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
														sum(coalesce(ssi.seiqtdtecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
													END
												END
											END
										as vlrsubacao
									FROM entidade.entidade t
									inner join entidade.funcaoentidade f on f.entid = t.entid
									left join entidade.entidadedetalhe ed on t.entid = ed.entid
									inner join entidade.endereco d on t.entid = d.entid
									left join territorios.municipio m on m.muncod = d.muncod
									left join par.escolas e on e.entid = t.entid
									INNER JOIN par.subacaoescolas se ON se.escid = e.escid
									INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano
									LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
									WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
									and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 and t.tpcid = 3
									GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico ) as foo )
							END AS valorsubacao,
							coalesce(es.emcpercentualemp,0) as percentualempenhado, 
							coalesce(es.emcvaloremp,0) as valorempenhado,
							'<input type=text id=id_'||sd.sbdid||' value='||
																			CASE WHEN p.protipoexecucao = 'C' 
																				THEN (99 - coalesce(es.emcpercentualemp,0)) 
																				ELSE (100 - coalesce(es.emcpercentualemp,0)) 
																			END 
							||' name=name_'||sd.sbdid||' size=6 onKeyUp=\"calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this) \" class=\"disabled\" readonly=readonly onfocus=\"this.select();\">
							<input type=hidden id=vlr_'||sd.sbdid||' name=vlr_'||sd.sbdid||' value='||
																									CASE WHEN p.protipoexecucao = 'C' 
																										THEN (99 - coalesce(es.emcpercentualemp,0)) 
																										ELSE (100 - coalesce(es.emcpercentualemp,0)) 
																									END
							
							||'>
							<input type=hidden id=anosubacao_'||sd.sbdid||' name=anosubacao_'||sd.sbdid||' value='||sd.sbdano||'>' 
							AS porcentagemaempenhar,
							CASE WHEN p.protipoexecucao = 'C' 
								THEN (99 - coalesce(es.emcpercentualemp,0)) 
								ELSE (100 - coalesce(es.emcpercentualemp,0)) 
							END AS porcentagem,
						
							sd.sbdano,
							sd.sbdid,
							sd.sbdplanointerno,
							sd.sbdptres, 
							iu.inuid
					FROM par.instrumentounidade 	iu
					INNER JOIN par.pontuacao 	po on po.inuid   = iu.inuid AND ptostatus <> 'I'
					INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid 
					INNER JOIN par.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'
					
					INNER JOIN execucaofinanceira.processos p  ON (p.muncod = iu.muncod) OR ( p.estuf = iu.estuf AND iu.muncod is null )
					
					INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid 
					LEFT JOIN execucaofinanceira.empenhocomposicao es on es.emccodigo = sd.sbdid
					LEFT JOIN execucaofinanceira.empenho 	e  ON e.empid = es.empid 
					WHERE p.proid = ".$obProcesso->proid." 
					AND iu.inuid = ".$obProcesso->inuid."
					AND s.frmid IN (13,6, 14,15)
					AND sd.ssuid IN (2, 12) -- aprovada pela comissão 
					AND sd.sbdid NOT IN (
						SELECT * FROM (
								SELECT 	CASE WHEN protipoexecucao = 'C'
									THEN
										CASE WHEN ( emcpercentualemp = 99) THEN subempenhadas END 
									ELSE
										CASE WHEN ( emcpercentualemp = 100) THEN subempenhadas END 
									END as sbdid
								FROM ( 
									SELECT 
									pr.protipoexecucao,
									ems.emcpercentualemp,	
									sd.sbdid as subempenhadas
									FROM execucaofinanceira.processos  pr
									INNER JOIN execucaofinanceira.empenho e on e.empnumeroprocesso = pr.pronumeroprocesso
									INNER JOIN execucaofinanceira.empenhocomposicao ems on ems.empid = e.empid
									INNER JOIN par.subacaodetalhe sd on sd.sbdid = ems.emccodigo
									INNER JOIN par.instrumentounidade iu on iu.muncod = pr.muncod
									-- INNER JOIN par.instrumentounidade iu on iu.estuf = pr.estuf AND muncod is null
									WHERE  iu.inuid = ".$obProcesso->inuid."
								) AS foo
							) as foo2	
						WHERE foo2.sbdid IS NOT NULL			
					) 	
				) AS dados";

		return $sql;
	}
	
	public function sqlListaDeSubacoes100Empenhado($obProcesso){	
		if( $obProcesso->esfera == 'M' ){
			$joinprocesso = " INNER JOIN execucaofinanceira.processos	p  ON p.muncod = iu.muncod ";
		} else {
			$joinprocesso = " INNER JOIN execucaofinanceira.processos p on p.estuf = iu.estuf AND p.muncod is null ";
		}
			
		$sql = "SELECT 	DISTINCT
				'' as botao, 
				e.empnumeroempenhosigef as numeroempenho, 
				s.sbadsc as descricaosubacao, 
				CASE WHEN sbacronograma = 1 
					THEN ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(sic.icoquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
										END
									END
								as vlrsubacao
						FROM par.subacaoitenscomposicao sic 
						WHERE sic.sbaid = s.sbaid
						AND sic.icoano = sd.sbdano
						GROUP BY sic.sbaid, sic.icovalidatecnico ) as foo )
					ELSE ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
							CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
								THEN -- escolas sem itens
									CASE WHEN se.sesvalidatecnico IS NULL -- antigos
									THEN 
										sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									END
								ELSE -- escolas com itens
									CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(ssi.seiqtd,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(ssi.seiqtdtecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
										END
									END
								END
							as vlrsubacao
						FROM entidade.entidade t
						inner join entidade.funcaoentidade f on f.entid = t.entid
						left join entidade.entidadedetalhe ed on t.entid = ed.entid
						inner join entidade.endereco d on t.entid = d.entid
						left join territorios.municipio m on m.muncod = d.muncod
						left join par.escolas e on e.entid = t.entid
						INNER JOIN par.subacaoescolas se ON se.escid = e.escid
						INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
						and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 and t.tpcid = 3
						GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico ) as foo )
				END AS valorsubacao,
				es.emcpercentualemp as percentualempenhado, 
				es.emcvaloremp as valorempenhado,
				sd.sbdano || ' ' as ano
		FROM par.instrumentounidade 	iu
		INNER JOIN par.pontuacao 	po on po.inuid   = iu.inuid
		INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid
		INNER JOIN par.subacao		s  on s.aciid   = a.aciid 
		
		INNER JOIN execucaofinanceira.processos p  ON (p.muncod = iu.muncod) OR ( p.estuf = iu.estuf AND iu.muncod is null )
		
		INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid 
		INNER JOIN execucaofinanceira.empenhocomposicao   es ON es.emccodigo = sd.sbdid
		INNER JOIN execucaofinanceira.empenho 	e  ON e.empid = es.empid
		WHERE 
		 true = (	SELECT DISTINCT CASE WHEN pr.protipoexecucao = 'C' 
						THEN ems.emcpercentualemp = 99 
						ELSE ems.emcpercentualemp = 100
						END as d
					FROM execucaofinanceira.processos pr
					INNER JOIN execucaofinanceira.empenho e on e.empnumeroprocesso = pr.pronumeroprocesso
					INNER JOIN execucaofinanceira.empenhocomposicao ems on ems.empid = e.empid
					WHERE  pr.proid =  ".$obProcesso->proid." 
					)
		AND  p.proid = ".$obProcesso->proid." 
		AND iu.inuid = ".$obProcesso->inuid;
		return $sql;
	}
	
	public function sqlListaDeSubacoes(){
		
	}
}
?>