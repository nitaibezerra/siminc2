<?php 
class PTA extends cls_banco {
	
	public $sqlPTA;
	public $arPerfil;
	private $param;	
	private $pteidPai;
	
	public function __construct(){
		parent::__construct();
		$this->arPerfil = $this->pegaPerfil();
		$this->pteidPai = null;
	}
	
	public function __destruct(){
		$this->sqlPTA = null;
		$this->arPerfil = null;
		$this->pteidPai = null;
	}
	
	/**
	 * metodo set()
	 * executada sempre que a propriedade for atribuída
	 *
	 * @param $propriedade = nome do atributo
	 * @param $valor = valor do atributo
	 */
	function __set($propriedade, $valor){
		$this->param[$propriedade] = $valor;
	}
	
	/**
	 * metodo get()
	 * executada sempre que a propriedade for requerida
	 *
	 * @param $propriedade = nome do atributo
	 * @return valor do atributo
	 */
	function __get($propriedade){
		return $this->param[$propriedade];
	}

	/**
	 * Função monta a lista de plano de trabalho cadastrados
	 *
	 * @param array $post
	 * @return array
	 */
	public function listaPlanoTrabalho(){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$obPost = (object) $_POST;
		
		#verifica se a requisição e uma consulta ou uma exportação para o excel.
		$exportar = ($obPost->exportar ? $obPost->exportar : 'false' );
		
		if (!$obPost->solicitarReformulacao) $obPost->solicitarReformulacao = "0";
		
		$residPossiveis = $this->recuperaResponsaveis( $_SESSION["usucpf"] );
		
		if($obPost->check_expandir){
			$emecod = (is_numeric($obPost->emecod) ? $obPost->emecod : '');
			$autid = (is_numeric($obPost->autid) ? $obPost->autid : '');
			$func = "carregaEmendaDetalhePTA(\"img_dimensao_' || ptr.ptrid || '\",\'' || ptr.ptrid || '\',\'".$emecod."\',\'".$autid."\');";
		} else {
			$func = "";
		}
		
		$sql = $obPTA->montaSQLListaPTA($obPost, $this->arPerfil, $residPossiveis );
		
		$arDados = $this->carregar( $sql );

		if( $arDados ){
			$dados_array = array();
			$dados_excel = array();
			foreach($arDados as $chave => $val){
				
				$boEmpenho = $val['boempenho'];

				if( $boEmpenho == 0 ){
					if( !$this->disabledPTA( $val['ptrid'] ) ){
						if( in_array( ADMINISTRADOR_INST, $this->arPerfil ) || in_array( SUPER_USUARIO, $this->arPerfil ) || in_array( INSTITUICAO_BENEFICIADA, $this->arPerfil ) ){
							$excluir = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a> <a href=\"#\" onclick=\"excluirPlanoTrabalho('".$val['ptrid']."');\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\">";
						} else {
							$excluir = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a> <a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";							
						}
					} else {
						if( in_array( ADMINISTRADOR_INST, $this->arPerfil ) || in_array( SUPER_USUARIO, $this->arPerfil ) || ( in_array( INSTITUICAO_BENEFICIADA, $this->arPerfil ) && $val['esdid'] == 52 ) ){
							$excluir = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a> <a href=\"#\" onclick=\"excluirPlanoTrabalho('".$val['ptrid']."');\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\">";
						} else {
							$excluir = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a> <a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
						}
					}
				} else {
					if( in_array( ADMINISTRADOR_INST, $this->arPerfil ) || in_array( SUPER_USUARIO, $this->arPerfil ) || ( in_array( INSTITUICAO_BENEFICIADA, $this->arPerfil ) && $val['esdid'] == 52 ) ){
						$excluir = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a> <a href=\"#\" onclick=\"excluirPlanoTrabalho('".$val['ptrid']."');\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\">";
					} else {
						$excluir = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a> <a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
					}		
				}
				
				
				
				if(!$val['esdid'] || $this->possuiPermissao($val['esdid'])) {
					$val['acoes'] = "<img style=\"cursor:pointer\" id=\"img_dimensao_{$val['ptrid']}\" src=\"/imagens/mais.gif\" onclick=\"carregaEmendaDetalhePTA(this.id,'{$val['ptrid']}','".(is_numeric($_REQUEST['emecod']) ? $_REQUEST['emecod'] : '')."','".(is_numeric($_REQUEST['autid']) ? $_REQUEST['autid'] : '')."');\" border=\"0\" /> <a href=\"#\" onclick=\"alterarPlanoTrabalho('{$val['ptrid']}',0,$obPost->solicitarReformulacao);\"> $excluir </a>";
				}else {
					$val['acoes'] = "<img style=\"cursor:pointer\" id=\"img_dimensao_{$val['ptrid']}\" src=\"/imagens/mais.gif\" onclick=\"carregaEmendaDetalhePTA(this.id,'{$val['ptrid']}','".(is_numeric($_REQUEST['emecod']) ? $_REQUEST['emecod'] : '')."','".(is_numeric($_REQUEST['autid']) ? $_REQUEST['autid'] : '')."');\" border=\"0\" /> <a href=\"#\" onclick=\"alterarPlanoTrabalho('{$val['ptrid']}',0,$obPost->solicitarReformulacao);\"> $excluir </a>";
				}
				if($val['anxid']){
					$val['acoes'].= "&nbsp;<img src=\"../imagens/anexo.gif\" title=\"Anexo\" border=\"0\">";
				}
				$impresso = '';
				
				if( $val['ptrimprimirpta'] == 't'){
					if( empty($val['ptrdataimpressao']) ){
						$impresso =  "<font color=\"red\">Pendente</font>";	
						$impressoExcel =  "Pendente";	
					} else {
						$impresso =  "Impresso";
						$impressoExcel = "Impresso";
					}
				} else {
					$impresso = '';
					$impressoExcel = '';
				}
				
				$boImpositivo = verificaOrcamentoImpositivo( $val['ptrid'] );
				if( $boImpositivo > 0 ){
					
					if( $_SESSION['exercicio'] > 2014 ){
						$filtroImp = ' and edi.edeid = ve.edeid';
					}
				
					#Valor impositivo de impedimento da emenda
					$valorImpositivo = $this->pegaUm("select
															coalesce(sum(edi.edivalor), 0) as valor
														from 
															emenda.ptemendadetalheentidade pte
															inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid
														    inner join emenda.emendadetalheimpositivo edi on edi.emdid = ve.emdid and edi.edistatus = 'A' $filtroImp
														where
															pte.ptrid = {$val['ptrid']}
														    and ve.edestatus = 'A'");
					
					$pedvalor = $this->pegaUm("select sum(pedvalor) from emenda.ptemendadetalheentidade where ptrid = {$val['ptrid']}");
					$ptrvalorconcedente = $pedvalor + $val['ptrvalorproponente'];
					$val['valortotal'] = ($ptrvalorconcedente - $valorImpositivo);
				}
				if( $_SESSION['exercicio'] >= '2014' ){
					$sql = "select tp.etodescricao 
							from emenda.v_emendadetalheentidade vede
		                    	inner join emenda.ptemendadetalheentidade pt on pt.edeid = vede.edeid
		                    	inner join emenda.emenda ee on ee.emeid = vede.emeid 
		                        inner join emenda.emendatipoorigem tp on tp.etoid = ee.etoid
	                        where pt.ptrid = {$val['ptrid']}
	                        	and tp.etostatus = 'A'
	                            and vede.edestatus = 'A'
	                            and ee.emeano = '{$_SESSION['exercicio']}'";
					$origemEmenda = $this->pegaUm($sql);
				} else {
					$origemEmenda = $val['sistema'];
				}
				
				if( $exportar == 'false' ){
					$dados_array[$chave] = array("acoes" => $val['acoes'],
												 "ptrid" => $val['ptrcod']."/".$_SESSION['exercicio'],
												 "enbcnpj" => $val['enbcnpj'],
												 "enbnome" => $val['enbnome'],
												 "estuf" => $val['estuf'],
												 "mundescricao" => $val['mundescricao'],
												 "resassunto" => $val['resassunto'],
												 "valorTotal" => $val['valortotal'],
												 "esddsc" => $val['esddsc'],
												 "imprimir" => $impresso,
												 "sistema" => $origemEmenda. $val['linha']);
				} else {
					// array da dados para excel
					$dados_excel[$chave] = array("ptrid" => $val['ptrcod']."/".$_SESSION['exercicio'],
												 "enbcnpj" => $val['enbcnpj'],
												 "enbnome" => $val['enbnome'],
												 "estuf" => $val['estuf'],
												 "mundescricao" => $val['mundescricao'],
												 "resassunto" => $val['resassunto'],
												 "valorTotal" => number_format($val['valortotal'], 2, ',', '.'),
												 "esddsc" => $val['esddsc'],
												 "imprimir" => $impressoExcel,
												 "sistema" => $origemEmenda);
				}
			}	
		}
		
		if( $exportar == 'true' ){
			return $dados_excel;
		} else {
			$cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", "Número do PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total", "Situação", "Imprimir PTA", "Origem PTA");
			$this->monta_lista_array($dados_array, $cabecalho, 50, 20, '', 'center', '');	
		}
		
		//return $dados_array;
	}
	
	/**
	 * Função verifica se usuario logado possui permissão na pagina
	 *
	 * @param string $categoria
	 * @return boolean
	 */
	public function possuiPermissao($categoria = 'geral') {
		
		include_once APPRAIZ . "www/emenda/permissoes_perfil.php";
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$pflcod = $obPTA->carregaPerfil();
		$retorno = false;
		if($pflcod) {
			foreach($pflcod as $perfil){
				if($perfil == SUPER_USUARIO) {
					$retorno = true;
					break;
				} else {
					if(!$retorno) $retorno = permissoesPerfil($perfil, $_REQUEST['modulo'], $categoria);
				}
			}
		}
		$retorno = ($retorno == NULL) ? false : $retorno;

		return $retorno;
	}

	/**
	 * Retorna o codigo do responsavel da emenda
	 *
	 * @return id do responsavel
	 */
	public function recuperaResponsaveis() {
		
		if( in_array( ADMINISTRADOR_INST, $this->arPerfil ) ){
			$sql = "SELECT resid FROM emenda.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."' AND rpustatus = 'A' AND resid is not null";
			$responsaveis = $this->carregarColuna($sql);
		} else {
			$responsaveis = array();
		}
	
		return $responsaveis;
	}
	
	/**
	 * Pega o perfil do usuario logado no sistema
	 *
	 * @return array de perfil
	 */	
	public function pegaPerfil(){	
		$obPTA = planoTrabalhoDAO::getInstance();
		$arPerfil = $obPTA->carregaPerfil();
		
		$arPerf = array();
		foreach ($arPerfil as $key => $p) {
			if( $p == SUPER_USUARIO ){
				$arPerf[] = SUPER_USUARIO;
				break; 
			} else {
				$arPerf[] = $p;				
			}
		}		
		return $arPerf;
	}
	
	/**
	 * Função desabilita inputs por perfil
	 *
	 * @return string
	 */
	public function disabledPTA( $ptrid = null ){
		$obPTA = planoTrabalhoDAO::getInstance();
		$pflcod = $obPTA->carregaPerfil();
		$retorno = false;
	
		if( ($pflcod && $_SESSION['exercicio'] != '2009') || $this->pegarEstadoAtual( $ptrid ) == EM_REFORMULACAO_PROCESSO  ) {
			foreach($pflcod as $perfil){
				if($perfil == SUPER_USUARIO || $perfil == ADMINISTRADOR_INST || $perfil == INSTITUICAO_BENEFICIADA) {
					$retorno = true;
					break;
				}
			}
		}
		if( !$retorno ){		
			return 'disabled="disabled"';
		} else {
			return '';
		}
	}
	
	/**
	 * Função desabilita inputs 
	 *
	 * @param string $categoria
	 */
	public function disabled($categoria = 'geral') {
		$retorno = '';
		
		if(!$this->possuiPermissao($categoria)) {
			$retorno = 'disabled="disabled"';
		}
		
		return $retorno;
	}
	
	/**
	 * Função exporta dados de uma consulta para excel
	 *
	 * @param array ou query $arSql
	 */
	public function exportarExcelPTA($arSql){
		ob_clean();
		$arSql = $arSql ? $arSql : array();
		$cabecalhoPTA = array("Número do PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total", "Situação", "Imprimir PTA");
		$this->sql_to_excel($arSql, 'relPTA', $cabecalhoPTA);
	}
	
	/**
	 * Carrega a entidade beneficiada
	 *
	 * @return html
	 */
	public function exibeInstituicaoBenefiada($enbid = null) {
		if(  in_array( INSTITUICAO_BENEFICIADA, $this->arPerfil ) ) {
			if($enbid) {
				$intituicao = $this->pegaUm("SELECT enbnome FROM emenda.entidadebeneficiada WHERE enbstatus = 'A' and enbid = ".$_SESSION["emenda"]["enbid"]);
		
				$intituicao = ($intituicao) ? $intituicao : 'Nenhuma Selecionada';
				
				$sql = "SELECT DISTINCT
						    count(ini.iniobras)
						FROM
							emenda.planotrabalho ptr
						    inner join emenda.entidadebeneficiada ent on ent.enbid = ptr.enbid
						    inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
						    inner join emenda.iniciativa ini on ini.iniid = pti.iniid    
						WHERE
						    ptr.ptrstatus = 'A' 
						    AND ptr.ptrexercicio = 2010
						    and ini.iniobras = 'S'
						    and ent.enbid = $enbid
						    AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')";
				if($this->pegaUm( $sql ) == 0){
					/*$html.= '<script>
							alert("Senhores dirigentes, \nInformamos que a efetivação do convênio dar-se-á mediante a apresentação da documentação de comprovação de dominialidade.");
						</script>';*/
				}
				
			} else {
				$intituicao = 'Nenhuma Selecionada';
			}
			$html .= '<table cellspacing="0" cellpadding="0" border="0" width="100%" style="border-top:solid 1px #8F8F8F;">
					<tbody>
						<tr>
							<td height="17" bgcolor="#e0e0e0" align="left" valign="middle">
								<font color="#696969" style="margin-right: 10px;">
								&nbsp;&nbsp;<b>Intituição Beneficiada:</b>
								'.$intituicao.'
								</font>
							</td>
						</tr>
						<tr><td></td></tr>
					</tbody>
				</table>';
			print "<br/>";
		}
		return $html;
	}
	
	/**
	 * Função carrega detalhe da emenda para o plano de trabalho
	 *
	 * @param array $post
	 */
	public function carregaEmendaDetalhePTA_Ajax($post){		
		$obPTA = planoTrabalhoDAO::getInstance();
		$sqled = $obPTA->SQLEmendaDetalhePTA_Ajax($post);
		
		echo '<table class="tabela" cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;">
				<tr>
					<td bgcolor="#e9e9e9" align="center" style=""><b>Recurso(s) Vinculado(s) ao Plano de Trabalho</b></td>
				</tr>
			</table>';
		$cabecalho = array("Recurso", "Autor", "Funcional Programática", "GND", "Mod", "Fonte", "Valor");
		$this->monta_lista_simples($sqled, $cabecalho, 20, 5);
				
		echo '<table class="tabela" cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;">
				<tr>
					<td bgcolor="#e9e9e9" align="center" style=""><b>Iniciativa(s) Vinculada(s) ao Plano de Trabalho</b></td>
				</tr>
			</table>';
		
		$sql = $obPTA->carregaIniciativaPTADAO( $post['ptrid'], '', true ); 
		
		$cabecalho = array("Código", "Iniciativa");
		$this->monta_lista_simples($sql, $cabecalho, 20, 5);
	} 

	/**
	 * Função para validação das session criadas
	 *
	 * @param $_SESSION $session
	 */
	public function validaSessionPTA($session){
		//if( empty($session)){
		if( !isset($session)){
			echo "<script>
					alert('Faltam dados na sessão sobre este PTA.');
					window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
				  </script>";
			die;
		}
	}
	
	
	public function cabecalhoPlanoTrabalho($ptrid) {
		
		if($ptrid) {
			$existePta = $this->pegaUm("SELECT count(*) FROM emenda.planotrabalho WHERE ptrid = ".$ptrid);
			
			if($existePta) {
				$obPTA = planoTrabalhoDAO::getInstance();
				
				$dadosPlanoTrabalho = $obPTA->cabecalhoPlanoTrabalhoDAO( $ptrid );
				$enbid = $dadosPlanoTrabalho[0]['enbid'];
				
				$dadosPlanoTrabalho[0]['cnpj'] = substr($dadosPlanoTrabalho[0]['cnpj'],0,2) . "." .
												 substr($dadosPlanoTrabalho[0]['cnpj'],2,3) . "." .
												 substr($dadosPlanoTrabalho[0]['cnpj'],5,3) . "/" .
												 substr($dadosPlanoTrabalho[0]['cnpj'],8,4) . "-" .
												 substr($dadosPlanoTrabalho[0]['cnpj'],12,2);
																 
				/*if( $federal == 'S' ){
					$dadosPlanoTrabalho[0]['valor'] = $dadosPlanoTrabalho[0]['ptrvalorconcedente'];
				}*/

				$boImpositivo = verificaOrcamentoImpositivo( $ptrid );
				if( $boImpositivo > 0 ){
					/* if( $_SESSION['exercicio'] >= '2014' ){
						$strIndicacao = verificaPrazoPreenchimentoEmenda( $enbid, $ptrid );
					} */
				
					if( $_SESSION['exercicio'] > 2014 ) $filtroImped = " and edi.edeid = ve.edeid ";
					
					#Valor impositivo de impedimento da emenda
					$arImpositivo = $this->pegaLinha("select
															coalesce(sum(edi.edivalor), 0) as valor, ve.etoid
														from 
															emenda.ptemendadetalheentidade pte
															inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid
														    inner join emenda.emendadetalheimpositivo edi on edi.emdid = ve.emdid and edi.edistatus = 'A' $filtroImped
														where
															pte.ptrid = $ptrid
														    and ve.edestatus = 'A'
														group by ve.etoid");
					$valorImpositivo = $arImpositivo['valor'];
					
					if( (int)$arImpositivo['etoid'] == 4 || (int)$arImpositivo['etoid'] == 1 ){
						$dadosPlanoTrabalho[0]['valor'] = ($dadosPlanoTrabalho[0]['ptrvalorconcedente'] - $valorImpositivo);
					} else {					
						$dadosPlanoTrabalho[0]['valor'] = ($dadosPlanoTrabalho[0]['valor'] - $valorImpositivo);
					}
				}
												 
				$titleValorRecurso = 'Composto pela soma do valor do recurso disponibilizado para a entidade pelo concedente e o valor da contrapartida informado pelo proponente anterior ao preenchimento do PTA (Plano de Trabalho)';
				$titleValorPTA = 'Composto pelos valores concedente e proponente detalhados nas iniciativas do PTA (Plano de Trabalho).';
				
				$emendaFederal = verificaEmendaFederal($ptrid);
				
				if( $emendaFederal == true ) $dadosPlanoTrabalho[0]['valor'] = (empty($dadosPlanoTrabalho[0]['valor']) ? $dadosPlanoTrabalho[0]['ptrvalorconcedente'] : $dadosPlanoTrabalho[0]['valor']);
				
				if( $dadosPlanoTrabalho[0]['valor'] == $dadosPlanoTrabalho[0]['ptrvalorconcedente'] ){
					$corFonte = 'color: blue;';
				} else {
					$corFonte = 'color: red;';
				}
				
				if( $dadosPlanoTrabalho[0]['sisid'] == 23 || $emendaFederal){
					$pendencia = "";
				} else {
					$pendencia = "<div style=\"float:right\" ><a onclick=\"window.open('emenda.php?modulo=principal/validacaoPTA&acao=A&popup=true','Indicador','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');\" href=\"#\" >Pendências PTA</a></div></div>";
				}
												 
				$cabecalho = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
								 <tbody>
								 	<tr>
								 		<td colspan=\"4\" class=\"subtitulocentro\">Dados do Plano de Trabalho</td>
								 	</tr>
									<tr>
										<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">Número do PTA / Exercício:</td>
										<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['ptrid']} / {$_SESSION['exercicio']}</td>
										<td style=\"text-align: right; width:20.0%;\" class=\"SubTituloEsquerda\">CNPJ:</td>
										<td style=\"width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\"><div><div style=\"float:left\">{$dadosPlanoTrabalho[0]['cnpj']}</div>$pendencia</td>
									</tr>
									<tr>
										<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nível de Ensino do PTA:</td>
										<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['resassunto']}</td>
										<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nome do Órgão ou Entidade:</td>
										<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['orgao_entidade']}</td>
									</tr>
									<tr>
										<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor total dos recursos:</td>
										<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; color: blue;\" class=\"SubTituloDireita\" title='$titleValorRecurso'>R$ ".number_format($dadosPlanoTrabalho[0]['ptrvalorconcedente'],2,',','.')."</td>
										<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município / UF:</td>
										<td style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['endereco']}</td>
									</tr>
									<tr>
										<td style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor do PTA:</td>
										<td colspan=\"3\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; $corFonte\" class=\"SubTituloDireita\" title='$titleValorPTA'>R$ ".number_format($dadosPlanoTrabalho[0]['valor'],2,',','.')."</td>
									</tr>
									$strIndicacao
								 </tbody>
								</table>";
								
				return $cabecalho;
			}
			else {
				echo "<script>
						alert('Número do Plano de Trabalho inválido.');
						window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
					  </script>";
				die;
			}
		} 
		else {
			echo "<script>
					alert('Você deve primeiramente selecionar um Plano de Trabalho.');
					window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
				  </script>";
			die;
		}
	}
	
	/**
	 * Função monta o cabeçalho da pagina definir recursos
	 *
	 * @param codigo $enbid
	 * @return html
	 */
	public function cabecalhoPTADefinirRecurso($enbid) {
		$obPTA = planoTrabalhoDAO::getInstance();
		$dadosPlanoTrabalho = $obPTA->carregaDadosCabecalhoDefinirRecurso($enbid);
		
		$dadosPlanoTrabalho[0]['cnpj'] = substr($dadosPlanoTrabalho[0]['cnpj'],0,2) . "." .
										 substr($dadosPlanoTrabalho[0]['cnpj'],2,3) . "." .
										 substr($dadosPlanoTrabalho[0]['cnpj'],5,3) . "/" .
										 substr($dadosPlanoTrabalho[0]['cnpj'],8,4) . "-" .
										 substr($dadosPlanoTrabalho[0]['cnpj'],12,2);
										 
		//$strIndicacao = verificaPrazoPreenchimentoEmenda( $enbid );
		
		$cabecalho = "<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
						 <tbody>
						 	<tr>
						 		<td colspan=\"2\" style=\"text-align: left;\"><b>Dados do Plano de Trabalho</b></td>
						 	</tr>
							<tr>
								<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">CNPJ:</td>
								<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['cnpj']}</td>
							</tr>
							<tr>
								<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nome do Órgão ou Entidade:</td>
								<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['orgao_entidade']}</td>
							</tr>
							<tr>
								<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município:</td>
								<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['municipio']}</td>
							</tr>
							<tr>
								<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">UF:</td>
								<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadosPlanoTrabalho[0]['uf']}</td>
							</tr>
							<tr>
								<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Exercício:</td>
								<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$_SESSION['exercicio']}</td>
							</tr>
							$strIndicacao
						 </tbody>
						</table>";
		
		return $cabecalho;
	}

	/**
	 * Função verifica se existe o numero de pta cadastrado
	 *
	 * @param integer $ptrid
	 * @return boolean
	 */
	public function verificaExistePTA( $ptrid ){
		if($ptrid){
			$sql  = "SELECT ptrid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}";
			$dado = $this->pegaUm( $sql );
		}
		
		if( !$dado ){
			echo "<script>
					alert('Número do Plano de Trabalho inválido.');
					window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
				  </script>";
			die;
		}		
	}

	/**
	 * Função carrega os registros cadastrados na pagina definir recursos
	 * 
	 * @param integer $ptrid
	 * @return arrat
	 */
	public function carregaDadosDefinirRecursos( $ptrid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		$arDadosPTA = $obPTA->SQLDadosDefinirRecursos( $ptrid );
		
		if($arDadosPTA){
			$arDadosPTA['ptrvalorproponente'] = number_format($arDadosPTA['ptrvalorproponente'], 2, ',', '.');		
			$arDadosPTA['ptrrendimentoaplicacao'] = number_format($arDadosPTA['ptrrendimentoaplicacao'], 2, ',', '.');		
		}	
		return $arDadosPTA;
	}
	
	/**
	 * Função controla os dados nivel de ensino beneficiado 
	 *
	 * @param integer $ptrid
	 * @param integer $enbid
	 * @param string $tipo
	 * @return array
	 */
	public function NivelEnsinoBeneficiado( $ptrid, $enbid, $tipo ){
		$obPTA = planoTrabalhoDAO::getInstance();
		$array = array();
		$dis = ($tipo == 'alteracao') ? 'disabled="disabled"' : '';
		$arTema = $obPTA->carregaNivelEnsinoBeneficiado( $ptrid, $enbid );
		$array[] = $arTema;
		$tema = '';
		if($arTema){
			$selected = count($arTema) == 1 ? 'checked' : '';
			$arResid = array();
			$arResassunto = array();
			foreach ($arTema as $key => $valor) {
				if( !in_array( $valor['mdeid'], $arResid) && !in_array( $valor['mdedescricao'], $arResassunto) )
					$tema.= '<label for="resp_'.$valor['mdeid'].'"><input type="radio" '.$dis.' name="responsavel" id="resp_'.$valor['mdeid'].'" value="'.$valor['mdeid'].'" onClick="carregaEmendasDisponiveis(this.value, \'\', '.$enbid.', '.$valor['emdid'].', '.$valor['resid'].');" '.$selected.'>'.$valor['mdedescricao'].'</label>';
				array_push( $arResid, $valor['mdeid']);
				array_push( $arResassunto, $valor['mdedescricao']);
			}			
		}else{
			$tema = '<span style="color:red;" > Não existe(m) emenda(s) disponível(eis) para preenchimento do Plano de Trabalho deste instituição. </span>';
		}
		$array[] = $tema;
		
		return $array;
	}
	
	/**
	 * Função Carrega Emendas Disponiveis
	 * Método usado para pesquisar registro no banco
	 * @param integer $resid - Código do Responsavel
	 * @return void 
	 * @access public
	 * @author Wesley Romualdo da Silva
	 * @since 09/09/2009
	 */
	public function carregaEmendasDisponiveisAjax($mdeid, $enbid){
		$obPTA = planoTrabalhoDAO::getInstance();
		$ptrid = $_SESSION['emenda']['ptrid'];
		/*$this->validaSessionPTA( $enbid );
		$this->validaSessionPTA( $ptrid );*/
		
		unset($_SESSION['emenda']['emdvalorTotal']);
		if( $ptrid || $enbid ){
			$arEmenda = $obPTA->carregaEmendasDisponiveisAjaxDAO( $ptrid, $mdeid, $enbid );	
		} else {
			$arEmenda = array();
		}
		$html = '';
		if($arEmenda){
			$html.= '<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Recursos Disponíveis</b></td>
						</tr>
					</table>
					<table id="tblform" class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<thead>
						<tr>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipo</strong></td>
							<td align="Center" class="title" width="20%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
							<td align="Center" class="title" width="15%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Funcional Programática</strong></td>
							<td align="Center" class="title" width="45%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>SubTítulo</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
						</tr>
						</thead>';
					$boFederal = false;
					foreach ($arEmenda as $key => $valor) {
						
						if( $valor['etoid'] == 4 ) $boFederal = true;
						
						$key % 2 ? $cor = "#dedfde" : $cor = "";
						$emedescentraliza = $valor['emedescentraliza'];
						$html.= '
						  <tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
						  	<td><img src="/imagens/mais.gif" border="0" id="img_dimensao_'.$valor['emeid'].'" onClick="carregaDetalheEmenda(this.id, '.$valor['emeid'].', \'listaEmendaDetalhe_'.$valor['emeid'].'\', \'trV_'.$valor['emeid'].'\');"> '.$valor['emecod'].'</td>
						  	<td>'.$valor['tipoemenda'].'</td>
						  	<td>'.$valor['autnome'].'</td>
						  	<td style="text-align: center;">'.$valor['fupfuncionalprogramatica'].'</td>
						  	<td>'.$valor['fupdsc'].'</td>
							<td style="text-align: right;">R$ '.number_format($valor['valor'],2,',','.').'</td>
						  </tr>
						<tr id="trV_'.$valor['emeid'].'" style="display: none">
					  	   <td colspan="5"><div id="listaEmendaDetalhe_'.$valor['emeid'].'" style="display: none">'.$this->carregaDetalheEmendaAjax($valor['emeid'], $ptrid).'</div></td>
					  	</tr>';
					}
					$_SESSION['emenda']['federal'] = $emedescentraliza;
					$total = $_SESSION['emenda']['emdvalorTotal'];
					$html.= '<tr><td colspan="6">&nbsp;</td></tr>
						  <tr bgcolor="#dedfde">
						  	<td colspan="6">
						  	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
								<tr>
									<td class="SubTituloDireita" style="width:20%;"><b>'.($boFederal ? 'Valor Total' : 'Valor Total do Concedente').':</b></td>
									<td><div id="emdvalorTotalDiv">R$ '.($total ? number_format($total, 2, ',', '.')  : '0,00').'</div>
										<input type="hidden" name="emdvalorTotal" id="emdvalorTotal" value="'.($total ? $total  : '0,00').'">
										<input type="hidden" name="vrlTotalEmenda" id="vrlTotalEmenda" value="0.00"></td>
								</tr>
							</table>
							</td>
						  </tr>';
		} else {
			$resassunto = $this->pegaUm("SELECT mdedescricao FROM emenda.modalidadeensino WHERE mdeid = $mdeid");
			$html.= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados registros para o nível de ensino '.$resassunto.'.</td></tr>';
			$html.= '</table>';
		}
		$html.= '</table>';
		
		echo $html;
	}
	
	/**
	 * Função carregaDetalheEmenda
	 * Método usado para mostrar os dados da emenda
	 * @param objeto idImg - Objeto do Formulario
	 * @param string div_nome - Nome da Div
	 * @param string tr_nome - Nome da tr
	 * @param int emeid - Codigo da Emenda
	 * @return void 
	 * @access public
	 * @author Wesley Romualdo da Silva
	 * @since 16/09/2009
	 */
	public function carregaDetalheEmendaAjax($emeid, $ptrid){
		$obPTA = planoTrabalhoDAO::getInstance();
		if( !isset($_SESSION['emenda']['dadosDetalheEntidade']) ){
			$_SESSION['emenda']['dadosDetalheEntidade'] = array();
		}
	
		$arEmendaD = $obPTA->carregaDetalheEmendaAjaxDAO( $emeid, $ptrid );
		$arDados = array();
		$html = '';
		if($arEmendaD){
			$html.= '<table id="tblform" class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="right">
			<thead>
			<tr>
				<td align="Center" class="title" width="10%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Seleciona</strong></td>
				<td align="Center" class="title" width="20%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
				<td align="Center" class="title" width="15%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Mod</strong></td>
				<td align="Center" class="title" width="45%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Fonte</strong></td>
				<td align="Center" class="title" width="11%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
			</tr>
			</thead>';
			$total = 0;
			
			foreach ($arEmendaD as $key => $valor) {
				$key % 2 ? $cor = "#FFFFFF" : $cor = "";
				if($valor['edeid']){
					if($ptrid)
						$check = 'checked="checked" disabled="disabled"';
					$total+= $valor['edevalor'];
					$value = $valor['coddetalhe']."_A_".($valor['pedid'] ? $valor['pedid'] : 'null');
				}else{
					$check = '';
					$value = $valor['coddetalhe']."_I_".($valor['pedid'] ? $valor['pedid'] : 'null');
				}
				
				if($valor['pedid']){
					$_SESSION['emenda']['dadosDetalheEntidade'][] = $valor['pedid']; 
				}
				
				$boTipoOrigem = $this->pegaUm("select coalesce(etoid, 0) from emenda.emenda e where emeid = $emeid");
				
				if( $valor['edevalor'] == '0.00' ){
					$disabled = 'disabled="disabled"';
				}
				
				if( date('Y-m-d') >= date('Y').'04-01' && $valor['emdimpositiva'] == '6' && $boTipoOrigem == 1 ){
					$disabled = 'disabled="disabled"';
					$title = 'Prazo de vigência para elaboração expirado em '.'01/04/'.date('Y');
				}
				
				$html.= '
				  <tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
				  	<td style="text-align: center;">
				  		<input id="emdid_'.$valor['emdid'].'" type="checkbox" '.$check.' '.$disabled.' name="emdid_'.$valor['emdid'].'" title="'.$title.'" value="'.$value.'" onclick="somaEmendaDetalhe(this,\''.$valor['edevalor'].'\', \''.$emeid.'\', \''.$boTipoOrigem.'\', \''.$_SESSION['exercicio'].'\');"/></td>
				  	<td style="text-align: center;">'.$valor['gndcod1'].'</td>
				  	<td style="text-align: center;">'.$valor['modalidade'].'</td>
				  	<td style="text-align: center;">'.$valor['fonte'].'</td>
					<td style="text-align: right;">
						<input type="hidden" name="vlrrecurso[]" id="vlrrecurso_'.$valor['emdid'].'" value="'.( ($valor['emetipo'] == 'E') ? $valor['edevalor'] : '0.00' ).'">
						R$ '.number_format($valor['edevalor'],2,',','.').'</td>
				  </tr>';
			}
			
			$_SESSION['emenda']['emdvalorTotal'] += $total;
			
		} else {
			$html.= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			$html.= '</table>';
		}
		$html.= '</table>';
		
		return $html;
	}
	
	public function insereFilhosPTA( $ptrid = null, $ptridpai = null, $ptiid = null, $pteid = null, $prdid = array(), $ptbid = null, $esbid = null, $exfid = null, $pmcid = null, $ptkid = null ){
		$obPTA = planoTrabalhoDAO::getInstance();
		#Tabela planotrabalho
		if( !$ptrid ){
			$ptrid = $obPTA->inserePlanoTrabalhoFilhoDAO( $ptridpai );
			$msg .= ( $ptrid ? '' : '\nNão foi possivel inserir o plano de trabalho' );
			$_SESSION["emenda"]["ptrid"] = $ptrid;
		}
		if( $ptridpai ){
			#Tabela ptemendadetalheentidade
			$pedid = $obPTA->inserePtemendadetalheentidadeFilhoDAO( $ptrid, $ptridpai );
			$msg .= ( $pedid ? '' : '\nNão foi possivel vincular o plano de trabalho ao detalhe da entidade' );
			
			$sql = "SELECT ptiid FROM emenda.ptiniciativa WHERE ptrid = $ptridpai";
			$arPtiniciativa = $obPTA->carregar( $sql );
			$arPtiniciativa = ( $arPtiniciativa ? $arPtiniciativa : array() );
	
			foreach ($arPtiniciativa as $vIni) {
				$ptiidPai = $vIni['ptiid'];
				#Tabela ptiniciativa
				if( !$ptiid ){
					$ptiid = $obPTA->inserePtiniciativaFilhoDAO( $ptrid, $ptiidPai, $ptridpai );
					$msg .= ( $ptiid ? '' : '\nNão foi possivel vincular a iniciativa ao plano de trabalho' );
				}
				#Tabela ptiniciativaEspecificacao
				//if( !$pteid ){
					$filtro = ( $this->pteidPai ? 'and pteid <> '.$this->pteidPai : '');					
					$arPtespecificacao = $obPTA->carregar( "SELECT pteid FROM emenda.ptiniciativaespecificacao WHERE ptiid = $ptiidPai and ptestatus = 'A' $filtro ");
					$arPtespecificacao = ( $arPtespecificacao ? $arPtespecificacao : array() );

					foreach ($arPtespecificacao as $vEsp) {
						$pteidPai = $vEsp['pteid'];
						
						
						$pteidNovo = $obPTA->inserePtiniciativaEspecificacaoFilhoDAO( $ptiid, $pteidPai, $pteid );

						$msg .= ( $pteidNovo ? '' : '\nNão foi possivel vincular a especificação ao plano de trabalho' );
						#Tabela ptitemkit
						if( !$pteid ){
							$obPTA->inserePtitemkitFilhoDAO( $pteidNovo, $pteidPai, $ptkid );
							$boAlteraEspecf = ($ptkid) ? true : false; 
							$ptkid = '';
						}	
						//$msg .= ( $boRetorno ? '' : "\nNão foi possivel vincular os kit's a especificação" );
						
						#Tabela ptiniesprecurso
						$obPTA->inserePtiniesprecursoFilhoDAO( $pteidNovo, $pteidPai, $pedid, $pteid );
						$pteid = '';
						//$msg .= ( $boRetorno ? '' : "\nNão foi possivel vincular o recurso a especificação" );
					}
					if( $boAlteraEspecf )
						$obPTA->alteraValorUnitarioEspecificacaoDAO( $pteidNovo );
				/*} else {
					if( $this->pteidPai ){
						#Tabela ptitemkit
						$obPTA->inserePtitemkitFilhoDAO( $pteid, $this->pteidPai );
						$this->pteidPai = '';
					}
				}*/
						
				#Tabela ptiniciativabeneficiario
				$obPTA->inserePtiniciativabeneficiarioFilhoDAO( $ptiid, $ptiidPai, $ptbid );
				$ptbid = '';
				//$msg .= ( $boRetorno ? '' : '\nNão foi possivel vincular beneficiário ao plano de trabalho' );
				
				if( $prdid[0] ){
					#Tabela ptparcelainiciativa
					$obPTA->inserePtparcelainiciativaFilhoDAO( '', $ptiid, '', $ptiidPai, $prdid );
				}
			}
			
			if( !$prdid[0] ){
				$arPtParcela = $obPTA->carregar( "SELECT prdid FROM emenda.ptparceladesembolso WHERE ptrid = $ptridpai" );
				$arPtParcela = ( $arPtParcela ? $arPtParcela : array() );
				
				foreach ($arPtParcela as $v) {
					$prdidPai = $v['prdid'];
					
					#Tabela ptparceladesembolso
					$prdid = $obPTA->inserePtparceladesembolsoFilhoDAO( $ptrid, $ptridpai, $prdidPai );
					$msg .= ( $prdid ? '' : '\nNão foi possivel vincular as cronograma desembolso ao plano de trabalho' );
					
					#Tabela ptparcelainiciativa
					$obPTA->inserePtparcelainiciativaFilhoDAO( $prdid, $ptiid, $prdidPai, $ptiidPai );
					//$msg .= ( $boRetorno ? '' : '\nNão foi possivel vincular as parcelas iniciativas ao cronograma de desembolso' );
				}
			} else {
				#Tabela ptparceladesembolso
				$obPTA->inserePtparceladesembolsoFilhoDAO( $ptrid, $ptridpai, $prdidPai, $prdid );
				//$msg .= ( $prdid ? '' : '\nNão foi possivel vincular as cronograma desembolso ao plano de trabalho' );
			}
				
			#Tabela ptescolasbeneficiadas
			if( !$esbid[0] ){
				$obPTA->inserePtescolasbeneficiadasFilhoDAO( $ptrid, $ptridpai );
				//$msg .= ( $boRetorno ? '' : '\nNão foi possivel vincular as escolas beneficiadas ao plano de trabalho' );
			} else {
				$obPTA->inserePtescolasbeneficiadasFilhoDAO( $ptrid, $ptridpai, $esbid );
			}
			
			if( !$exfid ){
				$exfid = $obPTA->insereExecucaoFinanceiraFilhoDAO( $ptrid, $ptridpai, $pedid );
				$obPTA->insereptExecfinanceiraHistoricoFilhoDAO( $exfid, $ptridpai );
			}
			
			$pmcid = $obPTA->inserePtminutaConvenioFilhoDAO( $ptrid, $ptridpai );
			$pmcid = $pmcid ? $pmcid : 'null';			
			$obPTA->insereIntervenienteConvenioFilhoDAO( $pmcid, $ptridpai );
			
			if( $pmcid ) $obPTA->inserePtvigenciaFilhoDAO( $ptrid, $ptridpai, $pmcid );
			
			$refid = $obPTA->insereptMinutaReformulacaoFilhoDAO( $ptrid, $ptridpai, $pmcid );
			$refid = $refid ? $refid : 'null';
			$obPTA->insereptReformulatiposFilhoDAO( $refid, $ptridpai );
			
			$obPTA->insereptPtpublicacaoFilhoDAO( $refid, $pmcid, $ptridpai );
			$anaid = $obPTA->insereptAnaliseFilhoDAO( $ptrid, $ptridpai );
						
			$cocid = $obPTA->insereContaCorrenteFilhoDAO( $ptrid, $ptridpai );
			if( $cocid ) $obPTA->insereptContaCorrenteHistoricoFilhoDAO( $cocid, $ptridpai );
			
			$mailid = $obPTA->inserePtmailFilhoDAO( $ptrid, $ptridpai );
			$mailid = $mailid ? $mailid : 'null';
			$anaid = $anaid ? $anaid : 'null';
			$obPTA->insereAnexoFilhoDAO( $ptrid, $anaid, $mailid, $ptridpai );
			
		}
		if(!$this->commit()){
			$this->rollback();
		}
		return $ptrid;
	}
	
	/**
	 * Função manter o plano de trabalho
	 *
	 * @param array de post $post
	 */
	public function inserePlanoTrabalho($post){
		
		$obPTA = planoTrabalhoDAO::getInstance();
		if( $_SESSION['exercicio'] >= '2011' ){			
			if( $post['enbcertificado'] == 'S' ){
				$sql = "UPDATE emenda.entidadebeneficiada SET 
							  enbcertificado = '{$post['enbcertificado']}',
							  enbcertificadovalido = '{$post['enbcertificadovalido']}' 
						WHERE 
						  enbid = ".$post['enbid'];
			} else {
				$sql = "UPDATE emenda.entidadebeneficiada SET 
							  enbnumprocesso = null,
							  enbvaldataini = null,
							  enbvaldatafim = null,
							  enbdatacertificado = null,
							  enbcertificado = '{$post['enbcertificado']}',
							  enbcertificadovalido = null 
						WHERE 
						  enbid = ".$post['enbid'];
			}
			$this->executar( $sql );
		}
	
		if( $post['ptrid'] ){
			$obPTA->alteraPlanoTrabalhoDAO($post);
			$this->insereEmendasDetalheAjax($post['dados'], $post['ptrid'], $obPTA);
		} else {
			$ptrid = $obPTA->inserePlanoTrabalhoDAO( $post );
			$this->insereEmendasDetalheAjax($post['dados'], $ptrid, $obPTA);			
			$_SESSION["emenda"]["ptrid"] = $ptrid;
			if( !empty($post['guia_check']) ){ 
				if(cadastraGuiaPTA($ptrid, $post)){
					$this->imprimirMenssagem( 'Falha na inserção das guias', 'principal/alteraDefinirRecursoPTA' );
				}	
			}
		}
	}
	
	public function pegaNumPTA( $ptrid ){		
		$sql = "SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = $ptrid and ptrstatus = 'A'";
		return $this->pegaUm( $sql );
	}
	
	/**
	 * Função vincula detalhe da emenda na entidade do pta 
	 *
	 * @param array $array
	 * @param integer $ptrid
	 */
	public function insereEmendasDetalheAjax($array, $ptrid, $obPTA){
		
		$arDados = $_SESSION['emenda']['dadosDetalheEntidade'];
		$arEmenda = explode('|', $array);
		
		$arPed = array();

		if($arEmenda && $arEmenda[0] != "") {
			
			foreach ($arEmenda as $arEmenda) {
				
				$separador = explode('_', $arEmenda);
				$edeid = $separador[0]; 
				$status = $separador[1]; 
				$pedid = $separador[2];
				$arPed[] = $pedid;
				
				//if($status == "I"){
					$obPTA->insereEmendasDetalheAjaxDAO( $ptrid, $edeid );
				/*}else{
					$obPTA->alteraEmendasDetalheAjaxDAO($ptrid, $edeid, $pedid);
				}*/
			}
		}
		ob_clean();
		if($this->commit()) {
			// cria o docid (workflow) do PTA
			$docid = $this->criarDocumento( $ptrid, $obPTA );
			echo $this->pegaNumPTA( $ptrid ).'/'.$_SESSION['exercicio'];
		} else {
			echo 0;
		}		
	}

	/**
	 * Função cria o documento no workflow
	 *
	 * @param integer $ptrid
	 * @param objeto $obPTA
	 * @return docid
	 */
	public function criarDocumento( $ptrid, $obPTA = null ) {		
		$docid = $obPTA->pegarDocid($ptrid);
		
		if( ! $docid ) {
			
			$boImpositivo = verificaOrcamentoImpositivo( $ptrid );
			
			if( $boImpositivo > 0 ){
				$tpdid = TPDID_EMENDA_IMPOSITIVO;
				$docdsc = "Cadastro de PTA (emendas orçamento impositivo) - n°" . $ptrid;
			} else {
				// recupera o tipo do documento
				$tpdid = TPDID_EMENDAS;				
				// descrição do documento
				$docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;
			}
			
			// cria documento do WORKFLOW
			$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
	
			// atualiza o plano de trabalho
			$sql = "UPDATE
						emenda.planotrabalho
					SET 
						docid = ".$docid." 
					WHERE
						ptrid = ".$ptrid;
	
			$this->executar( $sql );
			$this->commit();
		}
		
		return $docid;
	}
	
	/**
	 * Função verifica se existe emenda para o responsavel
	 *
	 * @param integer $resid
	 */
	public function verificaExistenciaEmendaAjax($resid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		echo $obPTA->verificaExistenciaEmendaAjaxDAO( $resid );
	}
	
	/**
	 * Função carrega a porcentagem da contra-partida
	 *
	 * @param unknown_type $resid
	 * @param unknown_type $enbid
	 */
	public function carregaPorcentagemContrapartidaAjax($mdeid, $enbid, $emdid) {
		
		if( $_SESSION['emenda']['federal'] == 'S' ){
			echo 'federal';
		} else {
			$resid = $this->pegaUm("SELECT resid FROM emenda.modalidadeensino WHERE mdeid = $mdeid");
			$sql = "SELECT ebccontrapartida FROM emenda.entbeneficiadacontrapartida WHERE ebcexercicio = '".$_SESSION["exercicio"]."' AND enbid = ".$enbid;
			$porcentagem = $this->pegaUm($sql);
			if(!$porcentagem && ($emdid != 'undefined' && !empty($emdid)) ) {
				$sql = "SELECT DISTINCT rmc.rmccontrapartida FROM emenda.emendadetalheentidade ede 
							inner join emenda.emendadetalhe  ed on ed.emdid = ede.emdid
							inner join emenda.responsavelmodcontrapartida rmc on rmc.mapcod = ed.mapcod and rmc.resid = $resid
						WHERE ede.enbid = $enbid and ed.emdid = '$emdid' and rmc.rmcano = '{$_SESSION['exercicio']}'";
				if( !empty($enbid) && !empty($emdid) && !empty($resid) )
					$porcentagem = $this->pegaUm($sql);
			}
			if($porcentagem)
				echo number_format($porcentagem, 2, ",", ".");
			else{
				if( $resid ){
					$sql = "SELECT rescontrapartidapadrao FROM emenda.responsavel WHERE resid = ".$resid." AND resstatus = 'A'";
					$porcentagem = $this->pegaUm($sql);
				}
				if( $porcentagem ){
					echo number_format($porcentagem, 2, ",", ".");
				} else 
					echo "erro";
			}
		}
	}
	
	public function buscaEmendaDescentraliza( $ptrid, $enbid = '', $emecod = '', $resid = '' ){
		
		$sql = "SELECT
				  	e.emedescentraliza
				FROM
				  	emenda.emenda e
				  	INNER JOIN emenda.emendadetalhe ed ON (e.emeid = ed.emeid) 
				  	INNER JOIN emenda.emendadetalheentidade ede ON (ed.emdid = ede.emdid)
				  	LEFT JOIN (SELECT ped.pedid, ped.edeid, ped.ptrid
				               FROM emenda.ptemendadetalheentidade ped
				    				inner join emenda.planotrabalho ptr
				                    	on (ptr.ptrid = ped.ptrid)
				               WHERE ptr.ptrstatus = 'A') ptede
				  		ON (ede.edeid = ptede.edeid)  
				WHERE
					(".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
				  	AND ede.edestatus = 'A'
				  	".( $resid ? 'AND e.resid = '.$resid : '')."
				  	AND ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $enbid)."
				  	AND ede.ededisponivelpta = 'S'
				  	AND e.emeano >= ".$_SESSION['exercicio'] ;

		$federal = $this->pegaUm( $sql );
		$_SESSION['emenda']['federal'] = $federal ;
		
		return $federal;
	}
	
	/**
	 * Função que verifica se usuario logado possui permissão de acesso a pagina
	 *
	 */
	public function verificaPermissao() {

		if( in_array(  INSTITUICAO_BENEFICIADA, $this->pegaPerfil() ) ) {
			$enbid = $this->verificaEntidadePta();
			
			if( $enbid ) {
				$_SESSION["emenda"]["enbid"] = $_SESSION["emenda"]["enbid"] ? $_SESSION["emenda"]["enbid"] : array();				
				if(!in_array($_SESSION["emenda"]["enbid"], $enbid)) {
					echo "<script>
						alert('Você não tem permissão para acessar esta tela.');
						history.back(-1);
					  </script>";
					die;
				}
			} else {
				echo "<script>
						alert('Você não tem permissão para acessar esta tela.');
						history.back(-1);
					  </script>";
				die;
			}
		}
	}
	
	/**
	 * Função pega a entidade vinculada ao usuario
	 *
	 * @return codigo da entidade
	 */
	public function verificaEntidadePta(){		
		if ( $this->testa_superuser() ){
			return true;
		}else{
			
			$sql = "SELECT
						ur.enbid
					FROM
						emenda.usuarioresponsabilidade ur
					INNER JOIN
						emenda.planotrabalho ep ON ep.enbid = ur.enbid
					WHERE
						ur.rpustatus = 'A' AND
						usucpf = '{$_SESSION["usucpf"]}' AND 
						pflcod = " . INSTITUICAO_BENEFICIADA;
			
			$enbid = $this->carregarColuna( $sql );
			return $enbid;		
		}	
	}
	
	/**
	 * Função que deleta as iniciativas vinculadas ao PTA
	 * @param integer $ptiid
	 * @return boolean
	 */
	public function deletaIniciativaPTA( $ptiid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		return $obPTA->deletaIniciativaPTADAO( $ptiid );		
	}
	
	/**
	 * Função pega o estado atual que se encontra o plano de trabalho
	 *
	 * @param integer $ptrid
	 * @return estado do pta
	 */
	public function pegarEstadoAtual( $ptrid ) {
		$obPTA = planoTrabalhoDAO::getInstance();
		$docid = $obPTA->pegarDocid( $ptrid );
	
		$estado = (integer) $obPTA->pegarEstadoAtualDAO( $docid );
		 
		return $estado;
	}
	
	/**
	 * Função verifica se a existe iniciativa vinculada ao pta com status ativo
	 *
	 * @param integer $ptrid
	 * @return boolean
	 */	
	public function verificaExclusaoIniciativaPTA($ptrid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$total = $obPTA->verificaExclusaoIniciativaPTADAO( $ptrid );
		
		if( $total == 0 ){
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 * Função carrega registro cadastrado na tabela ptiniciativa
	 *
	 * @param integer $ptrid
	 * @return array
	*/
	public function carregaDadosIniciativaPTA( $ptrid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		if( $this->possuiPermissao( $this->pegarEstadoAtual( $ptrid ) ) ) {
			if( $this->verificaExclusaoIniciativaPTA( $ptrid ) ){
				$acao = "<center>
							<a style=\"cursor:pointer;\" onclick=\"alteracao(\''||pti.ptiid||'\');\">
								<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
							</a>
							<a  style=\"cursor:pointer;\" onclick=\"removeracao(\''||pti.ptiid||'\');\">
								<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
							</a>
						</center>";
				
			} else {
				$acao = "<center>
						<a style=\"cursor:pointer;\" onclick=\"alteracao(\''||pti.ptiid||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<img src=\"/imagens/excluir_01.gif\" border=\"0\" title=\"Excluir\" style=\"cursor:pointer;\">
					</center>";
			}
			
		} else {
			$acao = "<center>
						<a style=\"cursor:pointer;\" onclick=\"alteracao(\''||pti.ptiid||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<img src=\"/imagens/excluir_01.gif\" border=\"0\" title=\"Excluir\" style=\"cursor:pointer;\">
					</center>";
		}
		
		return $obPTA->carregaDadosIniciativaPTA( $ptrid, $acao );
	}
	
	/**
	 * Função lista os beneficiarios vinculado a iniciativa e ao plano de trabalho
	 *
	 * @param integer $ptiid
	 * @return html
	 */
	public function ListarbeneficiariosAcaoPlanoTrabalho($ptiid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$dadosben = $obPTA->ListarbeneficiariosAcaoPlanoTrabalhoDAO( $ptiid );
		
		$html = '';
		if($dadosben){
			$html.= "<table class='listagem' width='100%'>
						<thead>
							<tr>
								<td width='60%'></td>
								<td align='center'><b>Rural</b></td>
								<td align='center'><b>Urbana</b></td>
								<td align='center'><b>Total</b></td>
							</tr>
						</thead>
						<tbody>";
			foreach( $dadosben as $benef ){
				$html.= "<tr>
							<td align='center'><b>".$benef['beneficiario']."</b></td>
							<td align='center'>".$benef['rural']."</td>
							<td align='center'>".$benef['urbana']."</td>
							<td align='center'>".$benef['total']."</td>
						<tr>";
			}
			
			$html.= "</tbody></table>";
		} else {
			$html.="<table class='listagem' width='100%'>
						<tr style='text-align: center;'>
							<td>-</td>
						</tr>
					</table>";
		}
		
		return $html;
	}
	
	/**
	 * Função monta o totalizador do beneficiario vinculado a iniciativa
	 *
	 * @param integer $ptrid
	 * @return html
	 */
	public function totalizaBeneficiariosPlanoTrabalho( $ptrid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$totalben = $obPTA->totalizaBeneficiariosPlanoTrabalhoDAO( $ptrid );
		$totalben = $totalben ? $totalben : array();
		
		$html = "<table class='listagem' width='100%'>";
		
		foreach( $totalben as $totalbenef ){
			$html.="<tr>
						<td align='center' width='60%'></td>
						<td align='center'><b>".$totalbenef['rural']."</b></td>
						<td align='center'><b>".$totalbenef['urbana']."</b></td>
						<td align='center'><b>".$totalbenef['total']."</b></td>
					<tr>";
		}
		$html.= '</table>';
		
		return $html;
	}
	
	/**
	 * Função monta um combo tipo de ensino vinculado a iniciativa
	 *
	 * @param integer $iniid
	 * @param integer $tpeid
	 * @return html
	 */
	public function montaComboIniciativaTipoEnsinoAjax($iniid, $tpeid){
		$obPTA = planoTrabalhoDAO::getInstance();
		if($iniid){
					  
			$arSelect = $obPTA->carregaIniciativaTipoEnsinoPTADAO( $iniid );
			
			$html.='<select id="tpeid" class="CampoEstilo" style="width: auto;" title="Tipo de Ensino" name="tpeid">
				<option value="">Selecione...</option>';
			
			if($arSelect){
				$tema = "";
				$count = sizeof($arSelect);
				foreach ($arSelect as $arSelect) {
					if($tpeid == $arSelect['codigo'] || $count == 1){
						$select = "selected=\"selected\"";
					}else{
						$select = "";
					}
					$html.= "<option value=\"".$arSelect['codigo']."\" $select>".$arSelect['descricao']."</option>";
				}
			}
			$html.='</select>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>';
		}else{
			$html = $this->monta_combo('tpeid',array(),'N','Selecione...',"",'','','','S','tpeid', '', '', 'Tipo de Ensino');
		}
		
		echo $html;
	}
	
	/**
	 * Função insere vinculo da iniciativa com plano de trabalho
	 *
	 * @param integer $ptrid
	 * @param string $descricao
	 * @param integer $iniid
	 * @param integer $tpeid
	 */
	public function insereIniciativaPTA( $ptrid, $descricao, $iniid, $tpeid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$ptiid = $obPTA->insereIniciativaPTADAO( $ptrid, $descricao, $iniid, $tpeid );
		
		return $ptiid;
	}
	
	/**
	 * Função altera vinculo da iniciativa com plano de trabalho
	 *
	 * @param integer $ptrid
	 * @param string $descricao
	 * @param integer $iniid
	 * @param integer $tpeid
	 * @param integer $ptiid
	 */
	public function alteraIniciativaPTA( $ptrid, $descricao, $iniid, $tpeid, $ptiid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		if( strlen( $descricao ) > '4000' ){
			echo "<script>alert('A descrição da iniciativa ultrapassa a quantidade de caracteres permitida!');
						location.href='emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A&ptaid=$ptiid';
				  </script>";
			die;
		} else {
			$obPTA->alteraIniciativaPTADAO( $ptrid, $descricao, $iniid, $tpeid, $ptiid );
			echo "<script>alert('Iniciativa alterada com sucesso!');
						  location.href='emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A&ptaid=$ptiid';
				  </script>";
			die;
		}
	}
	
	/**
	 * Função carrega dados da iniciativa
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function carregaDadosAcaoPTA( $ptiid ){
		if ( !empty( $ptiid ) ){
			$obPTA = planoTrabalhoDAO::getInstance();
			
			return $obPTA->carregaDadosIniciativaPTADAO( $ptiid );
		}
	}
	
/**
	 * Função monta sql de iniciativa vinculada ou não vinculada ao plano de trabalho
	 *
	 * @param integer $ptrid
	 * @return query
	 */
	public function montaSQLDadosAcaoPTA( $ptrid, $ptiid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		if( $ptiid ){
			return $obPTA->montaSQLDadosAcaoPTADAO( $ptrid );
		} else {
			return $obPTA->montaSQLDadosAcaoDAO( $ptrid );
		}
	}
	
	/**
	 * Função carrega cabeçalho da iniciativa especificação
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function carregaCabecalhoIniciativaEspecificacao( $ptiid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		if( !empty( $ptiid )  ){
			return $obPTA->carregaCabecalhoIniciativaEspecificacaoDAO( $ptiid );	
		} else {
			return '';
		}	
	}
	
	/**
	 * Função monta o cabecalho da listagem de especificação da iniciativa
	 *
	 * @return html
	 */
	public function cabecalhoEspecificacaoIniciativa(){
		if( $_SESSION['emenda']['federal'] == 'N' ){
			$compl = "<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"8%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Proponente</strong></td>";
		}
		$html = "<thead>
			<tr>
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"25%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Especificação da Iniciativa</strong></td>
				<td colspan=\"2\" align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Indicador Físico</strong></td>
				<td colspan=\"2\" align=\"Center\" class=\"title\" width=\"18%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Custo</strong></td>
				$compl
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"8%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Concedente</strong></td>
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data Inicial</strong></td>
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data Final</strong></td>
				<td rowspan=\"2\" align=\"Center\" class=\"title\" width=\"15%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ação</strong></td>
			</tr>
			<tr>
				<td align=\"Center\" class=\"title\" width=\"8%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Unidade de Medida</strong></td>
				<td align=\"Center\" class=\"title\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Quantidade</strong></td>
				<td align=\"Center\" class=\"title\" width=\"10%\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Unitário</strong></td>
				<td align=\"Center\" class=\"title\"
					style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"
					onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor Total</strong></td>
			</tr>
		</thead>";
		
		return $html;
	}
	
	/**
	 * Função monta a cambo das iniciativas que serão vinculadas a especificação 
	 *
	 * @param integer $ptiid
	 * @return html
	 */
	public function montaComboEspecificação( $ptiid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		if( $ptiid ){
			$arEspecif = $obPTA->montaComboEspecificacaoDAO( $ptiid );
		}
		$arEspecif = ( $arEspecif ? $arEspecif : array() );
		
		$html = '<select id="iceid" onchange="montaComboUnidade(this.value)" style="width: 250px;" class="CampoEstilo" title="Especificação da Iniciativa" name="iceid">';
		$html.= '<option value="">-- Selecione um vinculo --</option>';
		foreach ($arEspecif as $valor) {
			$html.= '<option value="'.$valor['codigo'].'" title="'.$valor['descricao'].'">'.$valor['descricao'].'</option>';
		}
		$html.= '</select>';
		
		return $html;
	}
	
	/**
	 * Função monta a lista de especificações cadastradas
	 *
	 * @param integer $ptiid
	 * @param integer $ptrid
	 * @return array
	 */
	public function listaEspecificacaoCadastradas( $ptiid, $ptrid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		$estado = $this->pegarEstadoAtual( $ptrid );
		
		$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );
		
		if( verificaPermissaoPerfil('pta', 'boolean', $estado, true) && (!in_array(RENDIMENTO_DE_APLICACAO, $arRrefid) || in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arRrefid) ) ) {
			if( $estado == EM_REFORMULACAO_PROCESSO ){
				$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptie.pteid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">  '  || 
					            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptie.pteid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
			} else {
				$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptie.pteid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">  '  || 
				            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptie.pteid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
			}
		} else {
			$acao = "( '<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" onclick=\"\" \" border=\"0\" alt=\"Ir\" title=\"Alterar\">  '  || 
				            '<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\" onclick=\"\" border=\"0\" alt=\"Ir\" title=\"Excluir\"></center>' )";
		}
		$this->validaSessionPTA( $ptiid );
		$this->validaSessionPTA( $ptrid );
		$arDados = $obPTA->listaEspecificacaoCadastradasDAO( $ptiid, $ptrid, $acao );		
		$totalizadorValorTotal		= 	0;
		$totalizadorValorProponente	=	0;
		$totalizadorValorConcedente	=	0;
		
		$arData = $obPTA->pegaDataEspecificacao( $ptiid );		
		$totalizadorDataInicio	=	$arData["min_dataini"];
		$totalizadorDataFim		=	$arData["max_datafim"];
		
		$html = '';
		if($arDados){
			$html.= '<tr>
			  			<th bgcolor="#E0E0E0" colspan="10" style="text-align: center"><b>Lista de Especificações</b></th>
			  		</tr>';
			$html.= $this->cabecalhoEspecificacaoIniciativa();
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				
				if($value['espkit'] == 't'){
					$unidade = '<a onclick="AlteraKit('.$value['pteid'].','.$value['iceid'].','."'true'".');" style="cursor: pointer;">'.$value['espunidademedida'].'</a>';
				}else{
					$unidade = $value['espunidademedida'];
				}
			
			$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
			$html.= '	<td><img src="/imagens/mais.gif" border="0" id="img_dimensao_'.$value['pteid'].'" 
							onclick="carregaDetalheEmenda(this.id, \''.$value['pteid'].'\', \'listaEmendaDetalhe_'.$value['pteid'].'\', \'trV_'.$value['pteid'].'\');">
							&nbsp;'.$value['espnome'].'</td>';
			$html.= '	<td>'.$unidade.'</td>';
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">'.$value['ptequantidade'].'</td>';
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['ptevalorunitario'],2,',','.').'</td>';
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['total'],2,',','.').'</td>';
			if( $_SESSION['emenda']['federal'] == 'N' ){
				$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['ptevalorproponente'],2,',','.').'</td>';
			}
			$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['concedente'],2,',','.').'</td>';
			$html.= '	<td style="text-align: center;">'.$value['dataini'].'</td>';
			$html.= '	<td style="text-align: center;">'.$value['datafim'].'</td>';
			$html.= '	<td style="text-align: center;">'.$value['acao'].'</td>';
			$html.= '</tr>';
			$html.= '<tr bgcolor="#FFFFFF" id="trV_'.$value['pteid'].'" style="display: none">';
			$html.= '	<td colspan="10" style="padding-left: 15px;padding-bottom: 25px;padding-top: 10px;"><div id="listaEmendaDetalhe_'.$value['pteid'].'" style="display: none">'.$this->carregaIniciativaEmendaAjax($value['pteid'], $ptrid).'</div></td>';
			$html.= '</tr>';
			
			// Soma os valores para o totalizador
			$totalizadorValorTotal			+= 	$value['total'];
			$totalizadorValorProponente		+=	$value['ptevalorproponente'];
			$totalizadorValorConcedente		+=	$value['concedente'];
			}
			$html.= '<tr>';
			$html.= '<td colspan="4" bgcolor="#cccccc" style="text-align: right;"><b>Totalizador:</b></td>';
			$html.= '<td style="text-align: center;color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorValorTotal,2,',','.').'</b></td>';
			if($_SESSION['emenda']['federal'] == 'N'){
				$html.= '<td style="text-align: center; color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorValorProponente,2,',','.').'</b></td>';
			}
			$html.= '<td style="text-align: center; color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorValorConcedente,2,',','.').'</b></td>';
			$html.= '<td style="text-align: center;"><b>'.$totalizadorDataInicio.'</b></td>';
			$html.= '<td style="text-align: center;"><b>'.$totalizadorDataFim.'</b></td>';
			$html.= '</tr>';
			$html.= '</table>';
		
			$html.= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr bgcolor="#ffffff">';
			$html.= '	<td><b>Total de Registros: '.count($arDados).'</b></td>';
			$html.= '	<td></td>';
			$html.= '</tr>';
			$html.= '</table>';
		}else{
			$html.= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr><td align="center" style="color:#cc0000;"></td></tr>';
			$html.= '</table>';
		}
		
		return $html;
	}
	
	/**
	 * Função monta o formulario para o cadastro das especificações que serão vinculadas ao plano de trabalho
	 *
	 * @param integer $ptiid
	 * @param integer $pteid
	 * @param integer $ptrid
	 * @return html
	 */
	public function listaPlanoEspecificacao($ptiid, $pteid, $ptrid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$this->validaSessionPTA( $ptiid );
		$this->validaSessionPTA( $ptrid );
		
		$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );	
		
		if( $this->pegarEstadoAtual( $ptrid ) == EM_REFORMULACAO_PROCESSO ){
			$sql = "SELECT
						to_char(min(pte.ptedatainicio), 'DD/MM/YYYY') as inicial,
						to_char(max(pte.ptedatafim), 'DD/MM/YYYY') as final
					FROM
						emenda.ptiniciativaespecificacao pte
					INNER JOIN
						emenda.ptiniciativa pti ON pti.ptiid = pte.ptiid
											   AND pti.ptrid = ".$ptrid."
					WHERE
						ptestatus = 'A'";
			
			$arData = $this->pegaLinha( $sql );
			$dataini = $arData['inicial'];
			$datafim = $arData['final'];
		}
		
		$disabilita = ( !verificaPermissaoPerfil('pta', 'boolean', $this->pegarEstadoAtual( $ptrid ), true) ? 'disabled="disabled"' : '');
		
		if( in_array(RENDIMENTO_DE_APLICACAO, $arRrefid) ){
			if(in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arRrefid)){
				$disabilita = '' ;
			} else {
				$disabilita = 'disabled="disabled"' ;
			}
		}
		
		$html = '<table width="100%" id="tbListaEspecificacao" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		$html.= $this->cabecalhoEspecificacaoIniciativa();
		$html.= '<tr bgcolor="#FCFDDB">';
		$html.= '	<td><div id="combo">'.$this->montaComboEspecificação( $ptiid ).'</div> <div id="texto" style="display: none"></div> </td>';
		$html.= '	<td valign="middle"><div id="unidade"></div><div id="unidade1"><input type="hidden" name="espkit" id="espkit" value=""></div></td>';
		$html.= '	<td style="text-align: center;">';
		$html.= '		<input id="ptequantidade" class="normal" type="text" title="Quantidade" onblur="MouseBlur(this);calculaTotal(\'\');" 
							onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
							onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" value="0" maxlength="20" size="6" name="ptequantidade"/>
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>';
		$html.= '	</td>';
		$html.= '	<td style="text-align: center;">'.campo_texto( 'ptevalorunitario', 'S', 'S', 'Valor Unitário', 18, 18, '[###.]###,##', '','','','','id="ptevalorunitario"','','',"calculaTotal(this.value); this.value=mascaraglobal('[###.]###,##',this.value)").'</td>';
		$html.= '	<td style="text-align: center;"><b><div id="total"></div></b></td>';
		if( $_SESSION['emenda']['federal'] == 'N' ){
			$html.= '	<td style="text-align: center;">'.campo_texto( 'ptevalorproponente', 'N', 'S', '', 18, 13, '[###.]###,##', '','','','','id="ptevalorproponente"','','',"this.value=mascaraglobal('[###.]###,##',this.value); verificaValorTotal(this.value);").'</td>';
		} else {
			$html.= '<input type="hidden" name="ptevalorproponente" id="ptevalorproponente" value="">';
		}
		$html.= '	<td style="text-align: center;"><b><div id="concedente"></div></b></td>';
		$html.= '	<td style="text-align: center;"><div id="dataini">'.($dataini ? '<input type="text" readonly="readonly" value="'.$dataini.'" name="ptedatainicioread" id="ptedatainicioread" size="12">
																					 <input type="hidden" value="'.$dataini.'" name="ptedatainicio" id="ptedatainicio" size="12">' : campo_data2('ptedatainicio', 'S','S','Data Inicial','','','') ).'</div></td>';
		$html.= '	<td style="text-align: center;">'.($datafim ? '<input type="text" readonly="readonly" value="'.$datafim.'" name="ptedatafimread" id="ptedatafimread" size="12"> 
																   <input type="hidden" value="'.$datafim.'" name="ptedatafim" id="ptedatafim" size="12">' : campo_data2('ptedatafim', 'S','S','Data Final','','','') ).'</td>';
		$html.= '	<td style="text-align: center;">';
		$html.= '		<input type="button" value="Salvar" name="btnIncluir" id="btnIncluir" '.$disabilita.' onclick="incluirEspecificacao();">';		
		$html.= '		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Cancelar();">';
		$html.= '	</td>';
		$html.= '</tr>';
		$html.= '<tr bgcolor="#FFFFFF" id="trRecurso" style="display: \'\';">';
		$html.= '	<td colspan="10" style="padding-left: 15px;padding-bottom: 25px;padding-top: 10px;"><div id="iniciativaEspecificacaoRecurso" style="display: \'\'">'.$this->iniciativaEspecificacaoRecursoAjax($ptiid, '', $ptrid).'</div></td>';
		$html.= '</tr>';
		$html.= $this->listaEspecificacaoCadastradas( $ptiid, $ptrid );		
		
		echo $html;
	}
	
	/**
	 * Função carrega as iniciativa vinculada a especificação e ao recurso
	 *
	 * @param integer $ptiid
	 * @param integer $pteid
	 * @param integer $ptrid
	 * @return html
	 */
	public function iniciativaEspecificacaoRecursoAjax( $ptiid, $pteid = '', $ptrid=null ){
		$obPTA = planoTrabalhoDAO::getInstance();
		if( $ptiid && $ptrid ){
			$arDados = $obPTA->iniciativaEspecificacaoRecursoDAO( $ptiid, $pteid, $ptrid );
		}
		$this->validaSessionPTA( $ptrid );
		$arDados = ( $arDados ? $arDados : array() );
		$html = '<span>
					  Abaixo estão listadas os recursos do PTA que poderão financiar a especificação. Indique o valor a ser utilizado em cada recurso:</span>
				<table id="tblform" class="listagem" width="80%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left">
					<thead>
					<tr>
						<td>&nbsp;</td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipo</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
						<td align="Center" class="title" width="25%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Total da Emenda</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>(-) Já Utilizado</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>(=) Valor Disponível</strong></td>
						<td align="Center" class="title" width="25%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
					</tr>
					</thead>';
		if($arDados){
			if(sizeof($arDados) == 1){
				$boValor = 'true';
				$html.= '<input type="hidden" name="boPedid" id="boPedid" value="'.$arDados[0]['pedid'].'">';
			}else{
				$boValor = 'false';
			}
			$html.= '<input type="hidden" name="boValor" id="boValor" value="'.$boValor.'">';
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				$pervalorUtilizado = $value['pervalorutilizado'] - $value["pervalor"];
				$value["pervalor"] = number_format($value["pervalor"] , 2, ',', '.');
								
				$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\';">
							<td><img src="/imagens/seta_filho.gif" border="0"></td>
							<td style="text-align: center;">'.$value['emecod'].'</td>
							<td style="text-align: center;">'.$value['tipoemenda'].'</td>
							<td style="text-align: center;">'.$value['gndcod'].'</td>
						  	<td style="text-align: center;">'.$value['autnome'].'</td>
						  	<td style="text-align: center;">'.number_format($value['pedvalor'],2,',','.').'</td>
						  	<td style="text-align: center;">'.number_format($pervalorUtilizado,2,',','.').'</td>
						  	<td style="text-align: center;">'.number_format( $value['pedvalor'] - $pervalorUtilizado,2,',','.').'</td>
						  	<td style="text-align: center;">
						  			<input id="pervalor_'.$value['pedid'].'" class="normal" type="text" title="" style="width: 43ex;" onblur="MouseBlur(this); formataValor(this); calculaEmendaEspecificacaoRecurso(); populaHidden(this.value, '.$value['pedid'].'); this.value=mascaraglobal(\'[###.]###,##\',this.value)" 
						  				onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);"
						  				onkeyup="this.value=mascaraglobal(\'[###.]###,##\',this.value);" value="'.$value["pervalor"].'" maxlength="15" size="41" name="pervalor_'.$value['pedid'].'"/></td>
						  			<input type="hidden" name="pervalor['.$value['pedid'].']" id="pervalor['.$value['pedid'].']" value="">
						  			<input type="hidden" name="emecod['.$key.']" id="emecod['.$key.']" value="'.$value['emecod'].'">
						</tr>';
			}
			$html.= '<tr>
						<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">Total do concedente:</td>
						<td style="text-align: left;"><span id="concedente2">R$ 0,00</span></td>
					</tr>
					<tr>
						<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">(-)Total informado:</td>
						<td style="text-align: left;"><span id="informado">R$ 0,00</span></td>
					</tr>
					<tr>
						<td colspan="6" bgcolor="#F5F5F5" style="text-align: right;">(=)Diferença:</td>
						<td style="text-align: left;"><span id="restante">R$ 0,00</span></td>
					</tr>';
		} else{
			$html.= '<table width="80%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			$html.= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			$html.= '</table>';	
		}
		$html.= "</table>";
		
		if( $pteid ){
			echo $html;
		} else {
			return $html;
		}
	}
	
	/**
	 * Função monta combo de unidades cadastradas
	 *
	 * @param integer $aceid
	 * @return html
	 */
	public function montaComboUnidade($aceid){
		if($aceid){
			$obPTA = planoTrabalhoDAO::getInstance();
			$dados = $obPTA->montaComboUnidadeDAO( $aceid );
			
			$html = '';
			if($dados){
				$html.= $dados['espunidademedida'];
				$html.= '<input type="hidden" value="'.$dados['espunidademedida'].'" id="espunidademedida" name="espunidademedida">
						<input type="hidden" value="'.$dados['espvalorunitariopadrao'].'" id="espvalorunitariopadrao" name="espvalorunitariopadrao">
						<input type="hidden" value="'.$dados['espkit'].'" id="espkit" name="espkit">';
			}else{
				$html.= '<input type="hidden" value="'.$dados['espunidademedida'].'" id="espunidademedida" name="espunidademedida">';
			}
		}
		echo $html;
	}
	
	/**
	 * Função verifica o valor total do proponente
	 *
	 * @param integer $pteid
	 * @param integer $ptrid
	 * @return array
	 */
	public function verificaTotalProponentePTA($pteid, $ptrid){
		$obPTA = planoTrabalhoDAO::getInstance();
				
		$filtro = ($pteid ? " AND ptie.pteid <> ".$pteid : $pteid);
		
		return $obPTA->verificaTotalProponentePTADAO($ptrid, $filtro);
	}
	
	public function imprimirMenssagem( $msg, $modulo = '', $parametro = '' ){
		$html = "<html><head><script>alert('".$msg."');";
		if( $modulo ){
			$url = $_SESSION['sisarquivo'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
			$html .= "location.href='$url'";
		}
		$html.='</script></head><body></body></html>';
		echo $html;
		exit();
	}
	
	/**
	 * Função manter dados da especificação da iniciativa
	 *
	 * @param array $post
	 */
	public function manterEspecificacao($post){
		$obPTA = planoTrabalhoDAO::getInstance();
		$msg = "";		
		$msgBanco = "";
		$boReformulacao = ($post['boreformulacao'] ? true : false);
		$espkit = ($post['espkit'] ? $post['espkit'] : $post['espkit1']);
		if($espkit == "t"){
			$valorUnit = ($post['ptevalorunitarioH'] == 0 ? 1 : $post['ptevalorunitarioH']);
			$post['ptequantidadeH'] = ($post['ptequantidadeH'] == 0 ? 1 : $post['ptequantidadeH']);
		} else {			
			$valorUnit = ($post['ptevalorunitario'] ? $post['ptevalorunitario'] : number_format($post['espvalorunitariopadrao'], 2, ',', '.') );
		}
	
		$valorUnit = str_replace('.','', $valorUnit);
		$valorUnit = str_replace(',','.', $valorUnit);
		
		$Proponente = str_replace('.','', $post['ptevalorproponente']);
		$Proponente = str_replace(',','.', $Proponente);
		
		$Proponente = trim($Proponente);
		$valorUnit 	= trim($valorUnit);
		
		if($Proponente && !$boReformulacao){
			$arTotalProponente = $this->verificaTotalProponentePTA( $post['pteid'], $post['ptrid'] );
			$valorTotalProponente = $arTotalProponente['totalespecificacao'] + $Proponente;
			
			if($valorTotalProponente > $arTotalProponente['totalplanotrabalho']){
				$msg = "Observação: O somatório dos valores informados para o proponente (R$ ".number_format($valorTotalProponente,2,',','.').") está excedendo o seu valor total disponível (R$ ".number_format($arTotalProponente['totalplanotrabalho'],2,',','.').")!";
				$this->imprimirMenssagem( $msg, 'principal/insereEspecificacaoAcao' );
			}
		}
		
		//ver( $post,d );
		$valorDisponivel = $this->verificaValorDisponivelEmenda( $post, $post['pteid'] );

		if( $post['pteid'] ){
			$msgBanco = $obPTA->alteraIniciativaEspecificacao($post, $Proponente, $valorUnit);
		}else{
			$pteid = $obPTA->insereIniciativaEspecificacao( $post, $Proponente, $valorUnit );
			
			if( empty( $pteid ) ){
				$this->rollback();
				$this->imprimirMenssagem( 'Falha na inserção da especificação da iniciativa', 'principal/insereEspecificacaoAcao' );
			}		
		}
		$pteid = $pteid ? $pteid : $post['pteid'];
		$this->manterInespRecurso($pteid, $post,$obPTA );
		
		if( $boReformulacao ){
			$sql = "UPDATE emenda.planotrabalho SET ptrvalorproponente = (SELECT sum(ptevalorproponente) FROM emenda.ptiniciativaespecificacao WHERE ptiid = ".$post['ptiid'].") WHERE ptrid = ".$post['ptrid'];
			$this->executar( $sql );
		}
		
		if( $this->commit() ){
			$this->sucesso( 'principal/insereEspecificacaoAcao' );
		}	
	}
	
	public function manterInespRecurso($pteid, $post, $obPTA ){

		if( !is_array($post["pervalor"]) ) $post["pervalor"] = array();
		
		foreach( $post["pervalor"] as $chave=>$valor ){
			
			if (!empty($valor)){
				$valor = str_replace( ".", "",  $valor );
				$valor = str_replace( ",", ".", $valor );	
			}else{
				$valor = '0.00';
			}
	
			$perid = $obPTA->verificaRecursoCadastradoDAO( $pteid, $chave );
			if( !is_numeric( $perid ) && !empty( $perid ) ){
				$this->imprimirMenssagem( $perid, 'principal/insereEspecificacaoAcao' );
			}
			
			/*//Total da ptemendadetalheentidade
			$totalEmenda  = $obPTA->retornaValorEmendaDetalheEntidadeDAO($chave);
			//Total da ptemendadetalheentidade já utilizado em outras especificações
			$totalRecursoJaUtilizado = $this->verificaTotalRecursoJaUtilizado($chave, $perid);
			$totalRestante = $totalEmenda - $totalRecursoJaUtilizado;   
			$totalRecursoAUtilizar = $totalRecursoJaUtilizado + $valor;
	
			if($totalRecursoAUtilizar > $totalEmenda){
				$msg = ($msg ? $msg . '|' : $msg). "O valor informado na emenda de número ".$post['emecod'][$i]." (R$ ".number_format($valor,2,',','.').") ultrapassa o \nseu valor total disponível (R$ ".number_format($totalRestante,2,',','.').")!";
				echo $msg;
				exit();
			}else{*/		
				$retorno = '';
				//if( !$boInsereFilho ){
					if( $perid  ){
						$retorno = $obPTA->alteraInespRecurso( $pteid, $chave, $valor );
					}else{
						$retorno = $obPTA->insereInespRecurso( $pteid, $chave, $valor );
					}
				//}
				if( !is_numeric( $retorno ) ){
					$this->imprimirMenssagem( 'Erro na inserção do recurso da especificação', 'principal/insereEspecificacaoAcao' );
				}
			//}
			$i++;
				
		}
		return true;
	}
	
	public function verificaValorDisponivelEmenda( $post, $pteid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		if( !is_array($post["pervalor"]) ) $post["pervalor"] = array();
		$msg = '';
		$i = 0; 
		foreach( $post["pervalor"] as $chave=>$valor ){
			
			if (!empty($valor)){
				$valor = str_replace( ".", "",  $valor );
				$valor = str_replace( ",", ".", $valor );	
			}else{
				$valor = '0.00';
			}	
			$perid = $obPTA->verificaRecursoCadastradoDAO( $pteid, $chave );
			//Total da ptemendadetalheentidade
			$totalEmenda  = $obPTA->retornaValorEmendaDetalheEntidadeDAO($chave);
			//Total da ptemendadetalheentidade já utilizado em outras especificações
			$totalRecursoJaUtilizado = $this->verificaTotalRecursoJaUtilizado($chave, $perid, $post['ptrid']);

			$totalRestante = $totalEmenda - $totalRecursoJaUtilizado;   
			$totalRecursoAUtilizar = $totalRecursoJaUtilizado + $valor;
			
			if( round($totalRecursoAUtilizar,2) > round($totalEmenda,2) ){
				$msg = "O valor informado na emenda de número ".$post['emecod'][$i]." (R$ ".number_format($valor,2,',','.').") ultrapassa o seu valor total disponível (R$ ".number_format($totalRestante,2,',','.').")!";
				$this->imprimirMenssagem( $msg, 'principal/insereEspecificacaoAcao' );
			}
			$i++;				
		}
		return true;
	}
	
	/**
	 * Função verifica o total de recurso utilizado pelo plano de trabalho
	 *
	 * @param integer $pedid
	 * @param integer $perid
	 * @return float
	 */	
	
	public function verificaTotalRecursoJaUtilizado($pedid, $perid, $ptrid = ''){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		if($perid){
			$filtro = " AND perid <> $perid";
		}
		/*$ptridpai = pegaPaiPTA( $ptrid );
		$ptridpai =  ( !empty($ptridpai) ? " or pt.ptrid = $ptridpai " : '');*/
				
		return $obPTA->verificaTotalRecursoJaUtilizadoDAO( $pedid, $filtro);
	}
	
	/**
	 * Função monta a listagem dos recursos vinculados com a especificação
	 *
	 * @param integer $pteid
	 * @param integer $ptrid
	 * @return html
	 */
	public function carregaIniciativaEmendaAjax($pteid, $ptrid){
		$obPTA = planoTrabalhoDAO::getInstance();
		$arDados = $obPTA->carregaIniciativaEmendaDAO( $pteid, $ptrid );
		$html = '<table id="tblform" class="listagem" width="70%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left">
			<thead>
			<tr>
				<td>&nbsp;</td>
				<td align="Center" class="title" width="20%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Código</strong></td>
				<td align="Center" class="title" width="10"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipo</strong></td>
				<td align="Center" class="title" width="10"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
				<td align="Center" class="title" width="40%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Autor</strong></td>
				<td align="Center" class="title" width="30%"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
			</tr>
			</thead>';	
		
		if($arDados){
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
							<td><img src="/imagens/seta_filho.gif" border="0"></td>
							<td style="text-align: center;">'.$value['emecod'].'</td>
							<td style="text-align: center;">'.$value['tipoemenda'].'</td>
							<td style="text-align: center;">'.$value['gndcod'].'</td>
						  	<td style="text-align: center;">'.$value['autnome'].'</td>
						  	<td style="text-align: center;">R$ '.number_format($value['pervalor'], 2, ',', '.').'</td>
						</tr>';
			}
			
		} else{
			$html.= '<tr><td align="center" style="color:#cc0000;" colspan="3">Não foram encontrados Registros.</td></tr>';
		}	
		$html.= "</table>";

		return $html;
	}
	
	/**
	 * Função insere especificação com status P
	 *
	 * @param array $post
	 * @return boolean
	 */
	public function insereEspecificacaoPendente($post){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$pteid = $obPTA->insereEspecificacaoPendenteDAO( $post );
		echo $pteid;	
	}
	
	/**
	 * Função exclui inciativas especificações vinculadas ao plano de trabalho
	 *
	 * @param integer $pteid
	 * @return boolean
	 */
	public function excluirPlanoEspecificacao($pteid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		echo $obPTA->excluirPlanoEspecificacaoDAO( $pteid );		
	}
	
	/**
	 * Função carrega os registro de especificação vinculado ao plano de trabalho para alteração
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function alterarPlanoEspecificacao($pteid){
		$obPTA = planoTrabalhoDAO::getInstance();
		$arDados = array();
		if( $pteid ){
			$arDados = $obPTA->carregaPlanoEspecificacaoDAO( $pteid );
			
			$arDados['espkit'] = iconv("iso-8859-1","utf-8",$arDados['espkit']);
			$arDados['espnome'] = iconv("iso-8859-1","utf-8",$arDados['espnome']);
			$arDados['espunidademedida'] = iconv("iso-8859-1","utf-8",$arDados['espunidademedida']);
		}
		echo simec_json_encode( $arDados );
	}
	
	/**
	 * Função carrega recurso vinculado ao plano de trabalho
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function carregaPTARecurso($pteid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$arDados = $obPTA->carregaPTARecursoDAO( $pteid );
		echo simec_json_encode( $arDados );
	}
	
	/**
	 * Função exclui kit da especificação
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function excluirKitEspecif($pteid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		echo $obPTA->excluirKitEspecifDAO( $pteid );
	}
	
	/**
	 * Função verifica se existe kit cadastrado para a especificação
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function verificaExistenciaKit($pteid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		if($pteid){
			echo $obPTA->verificaExistenciaKitDAO( $pteid );
		}else{
			echo '-1';		
		}
	}

	/**
	 * Função verifica se existe pteid para o plano de trabalho selecionado, 
	 * 		caso não exista fecha a tela de cadastro de kit da especificação
	 *
	 * @param integer $pteid
	 * @param integer $ptrid
	 * @return boolean
	 */
	public function validaEspecificacaoPTA( $pteid, $ptrid ){
		if( !$pteid ){
			echo "<script>
					alert('Número da especificação e inválido.');
					window.close();
				  </script>";
			die;
		} else {
			if( is_numeric( $pteid ) ){
				$obPTA = planoTrabalhoDAO::getInstance();		    
				$boExiste = $obPTA->validaEspecificacaoPTADAO( $ptrid, $pteid );
			} else {
				$boExiste = 0;
			}
			
			if($boExiste == 0){
				echo "<script>
						alert('Este número da especificação e inválido para o plano de trabalho selecionado.');
						window.close();
					  </script>";
				die();
			} else {
				return true;
			}
		}
	}
	
	/**
	 * Função carrega dados da especificação para o kit da especificação
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function carregaDadosEspecificacaoKit( $pteid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		return $obPTA->carregaDadosEspecificacaoKitDAO( $pteid );
	}
	
	/**
	 * Função monta a lista de registro cadastrado na tabela ptitemkit
	 *
	 * @param integer $pteid
	 * @param boolean $read
	 * return html
	 */
	public function popupCadastraKit($pteid, $read = null){
		monta_titulo('Detalhamento dos ítens', '');		
		$obPTA = planoTrabalhoDAO::getInstance();
		$html = '';
		if($read){
			$html .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>				
							<td rowspan="2" align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Identificação dos Itens</strong></td>
								
							<td rowspan="2" align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Unidade de Medida</strong></td>
								
							<td rowspan="2" align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Quantidade</strong></td>
								
							<td colspan="2"  align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Estimativa de Custo</strong></td>
						</tr>
						<tr>
							<td align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Unitário</strong></td>
							<td align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Total</strong></td>
						</tr>
						</thead>';

				
						
				$arDados = $obPTA->carregaPtitemkitDAO( $pteid );
				
				if($arDados){
					foreach ($arDados as $key => $value) {
						$key % 2 ? $cor = "" : $cor = "#f7f7f7";
						
						$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\';">
									<td>'.$value['ptkdescricao'].'</td>
									<td>'.$value['ptkunidademedida'].'</td>
									<td style="text-align: center;">'.$value['ptkquantidade'].'</td>
									<td style="text-align: center;">R$ '.number_format($value['ptkvalorunitario'],2,',','.').'</td>
									<td style="text-align: center;">R$ '.number_format($value['total'],2,',','.').'</td>
								</tr>';
			
						// Soma os valores para o totalizador
						$totalQuantidade += $value['ptkquantidade'];
						$totalValorUnit += $value['ptkvalorunitario'];
						$totalizadorItens += $value['total'];
					}
					
					$html.= '<thead>
								<tr>
									<td colspan="2" style="text-align: right;"><b>Totalizador:</b></td>
									<td style="text-align: center;color: rgb(0, 102, 204);"><b>'.$totalQuantidade.'</b></td>
									<td style="text-align: center;color: rgb(0, 102, 204);"><b>R$ '.number_format($totalValorUnit,2,',','.').'</b></td>
									<td style="text-align: center;color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorItens,2,',','.').'</b></td>
								</tr>
								</thead>
							</table>
							<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
								<tr bgcolor="#ffffff">
									<td><b>Total de Registros: '.count($arDados).'</b></td>
									<td></td>
								</tr>
							</table>';
			}
		}else{
			$html.= '<input type="hidden" value="'.$pteid.'" name="pteid" id="pteid">
					<input type="hidden" name="hidValorTotal" id="hidValorTotal" value="">
					
					<table width="95%" id="tb_listaKit" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>				
							<td rowspan="2" align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Identificação dos Itens</strong></td>
								
							<td rowspan="2" align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Unidade de Medida</strong></td>
								
							<td rowspan="2" align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Quantidade</strong></td>
								
							<td colspan="2"  align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Estimativa de Custo</strong></td>
								
							<td rowspan="2" align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Ação</strong></td>
						</tr>
						<tr>
							<td align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Unitário</strong></td>
							<td align="Center" class="title"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Total</strong></td>
						</tr>
						</thead>';
			//campo_texto( 'ptkdescricao', 'S', 'S', 'Identificação dos Itens', 50, 500, '', '','','','','id="ptkdescricao"','','','')
			
			/**
			 * Solicitante: JÚLIO CEZAR DA C R VIANA
			 * Data: 23/12/2013 15:00
			 */
			$arAcao = $this->carregarColuna("select vf.acacod 
											from emenda.ptemendadetalheentidade ped
												inner join emenda.v_emendadetalheentidade ve on ve.edeid = ped.edeid
											    inner join emenda.v_funcionalprogramatica vf on vf.acaid = ve.acaid
											where
												ped.ptrid = {$_SESSION["emenda"]["ptrid"]}
											    and ve.edestatus = 'A'
											    and vf.acastatus = 'A'");
			$arAcao = $arAcao ? $arAcao : array();
			
			if( $_SESSION['exercicio'] >= '2012' && !in_array('0048', $arAcao) ){
			$sqlItem = "select  tipo||'_'||codigo as codigo, descricao from (
							SELECT 
														ipe.iteid as codigo,
													    pic.picdescricao as descricao,
							                            'P' as tipo
													FROM emenda.itempar gip
														inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
							                            inner join emenda.itempar_especificacao ipe on ipe.ipaid = gip.ipaid
													WHERE
														pic.picstatus = 'A'
							                            and ipe.espid in (select espid from emenda.iniciativaespecificacao ie
																			inner join emenda.ptiniciativaespecificacao pe on pe.iceid = ie.iceid where pe.pteid = $pteid and ie.icestatus = 'A')
														and ipe.iteid not in (select iteid from emenda.ptitemkit p where p.pteid = $pteid and iteid is not null)
													UNION
							                        SELECT
							                            io.itoid as codigo,
							                            pto.ptodescricao as descricao,
							                            'O' as tipo
							                        FROM
							                            emenda.itemobras io
							                            inner join obras.pretipoobra pto on pto.ptoid = io.ptoid
							                            inner join emenda.itempar_especificacao ipe on ipe.itoid = io.itoid
							                        WHERE
							                            pto.ptostatus = 'A'
							                            and ipe.espid in (select espid from emenda.iniciativaespecificacao ie
																			inner join emenda.ptiniciativaespecificacao pe on pe.iceid = ie.iceid where pe.pteid = $pteid and ie.icestatus = 'A')
														and io.itoid not in (select itoid from emenda.ptitemkit p where p.pteid = $pteid and itoid is not null)
							) as foo
							order by
								descricao";
			$arrItem = $this->carregar($sqlItem);
			} else {
				$arrItem = array();
			}
			
			$arRrefid = verificaTiposReformulacao( $_SESSION["emenda"]["ptrid"], 'codigo' );
			$disabled = 'S';
			$esdid = pegarEstadoAtual($_SESSION['emenda']['ptrid']);
			
			if( !in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arRrefid) && $esdid == EM_REFORMULACAO_PROCESSO ){ 
				$disabled = 'N';
			}
			
			$html.= '<tr>
				<td><div id="item" style="display: \'\'">'.($arrItem ? $this->monta_combo("iteid",$sqlItem, 'S','-- Selecione --','carregaDadosItem', '', '',250,'S','iteid', true, '', 'Identificação dos Itens') . '<input type="hidden" value="item" name="tipoitem" id="tipoitem">' : campo_texto( 'ptkdescricao', 'S', 'S', 'Identificação dos Itens', 50, 500, '', '','','','','id="ptkdescricao"','','','') . '<input type="hidden" value="desc" name="tipoitem" id="tipoitem">' ).'</div><div id="itemdesc" style="display: none"></div></td>
				<td>'.campo_texto( 'ptkunidademedida', 'S', 'S', 'Unidade de Medida', 30, 100, '', '','','','','id="ptkunidademedida"','','','').'<div id="unidade"></div></td>
				<td><input id="ptkquantidade" class="normal" type="text" title="Quantidade" style="width: 23ex;" onblur="MouseBlur(this); this.value=mascaraglobal(\'[#]\',this.value); calculaTotal(\'\');" 
						onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
						onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" value="0" maxlength="15" '.($disabled == 'N' ? 'readonly="readonly"' : '').' size="21" name="ptkquantidade"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				<td>'.campo_texto('ptkvalorunitario', 'S', $disabled, 'Valor Unitário', 20, 13, '[###.]###,##', '','','','','id="ptkvalorunitario"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value); calculaTotal('');").'<div id="valor"></div></td>
				<td style="text-align: center;"><div id="total">R$ 0,00</div></td>
				<td style="text-align: center;"><input type="button" value="Incluir" name="btnIncluir" id="btnIncluir" onclick="incluir();">
												<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Cancelar();"></td>
			</tr>';
			
			$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptkid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">'  || 
					            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptkid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
							
			$arDados = $obPTA->carregaPtitemkitDAO( $pteid, $acao );
			$valorTotal = 0;
			if($arDados){
				foreach ($arDados as $key => $value) {
					$key % 2 ? $cor = "" : $cor = "#f7f7f7";
					$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\'">
								<td>'.$value['ptkdescricao'].'</td>
								<td>'.$value['ptkunidademedida'].'</td>
								<td style="text-align: center;">'.$value['ptkquantidade'].'</td>
								<td style="text-align: center;">R$ '.number_format($value['ptkvalorunitario'],2,',','.').'</td>
								<td style="text-align: center;">R$ '.number_format($value['total'],2,',','.').'</td>
								<td style="text-align: center;">'.$value['acao'].'</td>
							</tr>';
					
					// Soma os valores para o totalizador
					$totalQuantidade += $value['ptkquantidade'];
					$totalValorUnit += $value['ptkvalorunitario'];
					$totalizadorItens += $value['total'];
					$valorTotal+=$value['total'];
				}
				
				$html.= '<tr>
							<td colspan="2" style="text-align: right; background: #E3E3E3"><b>Totalizador:</b></td>
							<td style="text-align: center;color: rgb(0, 102, 204); background: #E3E3E3"><b>'.$totalQuantidade.'</b></td>
							<td style="text-align: center;color: rgb(0, 102, 204); background: #E3E3E3"><b>R$ '.number_format($totalValorUnit,2,',','.').'</b></td>
							<td style="text-align: center;color: rgb(0, 102, 204); background: #E3E3E3"><b>R$ '.number_format($totalizadorItens,2,',','.').'</b></td>
						</tr>
			
					</table>
					<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<tr bgcolor="#ffffff">
							<td><b>Total de Registros: '.count($arDados).'</b></td>
							<td></td>
						</tr>
					</table>';
			}
			echo '<input type="hidden" name="hidValorTotalGeral" id="hidValorTotalGeral" value="'.number_format($valorTotal,2, ',', '.').'">';
		}
		$html.= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" >
					<tr>
						<td>&nbsp;</td>
					</tr>
					<tr bgcolor="#ffffff">
						<td style="text-align: center;"><input type="button" name="fechar" value="Fechar" onclick="javascript: fecharKit();"></td>
					</tr>
				</table>';
		
		echo $html;
	}
	
	/**
	 * Função manter os registros cadastrados na tabela ptitemkit
	 *
	 * @param array $post
	 * return string
	 */
	public function insereKitPlanoTrabalho($post){
		$obPTA = planoTrabalhoDAO::getInstance();
		ob_clean();
		if($post['ptkid'] ){
			$ret = $obPTA->alteraPtitemkitDAO( $post );
			
			if($ret == true){
				$obPTA->alteraValorUnitarioEspecificacaoDAO( $post['pteid'] );
			}
			echo trim($ret);		
		}else{
			/*if( $post['ptkid'] && empty( $post['ptridpai'] ) && empty($post['refid']) ){
				$obPTA->alteraValorUnitarioEspecificacaoDAO( $post['pteid'] );
				if($ret == true){
					$obPTA->alteraValorUnitarioEspecificacaoDAO( $post['pteid'] );
				}
				echo $ret;
			} else {*/
				$total = 0; //$obPTA->verificaExisteptitemkitDAO( $post );
				if($total == 0){
					
					$ret = $obPTA->inserePtitemkitDAO( $post );
					if( is_numeric( $ret )){						
						/*if( empty( $post['ptridpai'] ) && !empty($post['refid']) ){							
							$_SESSION["emenda"]["ptkid"] = $post['ptkid'];
							$ptrid = $this->insereFilhosPTA('', $post['ptrid'], '', '', '', '', '', '', '', $ret );
							$_SESSION["emenda"]["ptridpai"] = $post["ptrid"];
							echo 'filho';
						} else {*/
							$obPTA->alteraValorUnitarioEspecificacaoDAO( $post['pteid'] );
							echo 1;
						//}
					} else {						
						echo trim($ret);
					}
				}else{
					echo 2;
				}
			//}
		}
	}
	
	/**
	 * Função carrega os registro da tabela ptitemkit para alteração
	 *
	 * @param integer $ptkid
	 * return array
	 */
	public function alterarKitEspecificacao($ptkid){
		$obPTA = planoTrabalhoDAO::getInstance();
		$arDados = $obPTA->carregaAlteracaoPtitemkitDAO( $ptkid );
		
		$arDados['ptkdescricao'] = iconv("iso-8859-1","utf-8",$arDados['ptkdescricao']);
		$arDados['ptkunidademedida'] = iconv("iso-8859-1","utf-8",$arDados['ptkunidademedida']);
		
		echo simec_json_encode( $arDados );
	}
	
	/**
	 * Função excluir os kits vinculados a especificação
	 *
	 * @param integer $ptkid
	 * @param integer $pteid
	 */
	public function excluirKitEspecificacao($ptkid, $pteid){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		$ret = $obPTA->excluirPtitemkitDAO( $ptkid );
	
		if($ret == true){
			$obPTA->alteraValorUnitarioEspecificacaoDAO( $pteid );
		}
		echo $ret;
	}
	
	/**
	 * Função manter beneficiario
	 *
	 * @param integer $ptiid
	 * @param array $post
	 */
	public function manterBeneficiariosPTA( $ptiid, $post, $boReformulacao = false ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		if( !empty( $post['ptbid'] ) ){
			#exclui as inciativas vinculadas ao plano de trabalho
			$obPTA->excluirPtiniciativabeneficiarioDAO( $ptiid );
		}

		#carrega os dados do beneficiario cadastrados
		if( !empty($post['iniid']) ) $dados = $obPTA->carregaBeneficiarioDAO( $post['iniid'] );
		$dados = $dados ? $dados : array();
		
		/*if( !empty( $post['ptbid'] ) && empty( $post['ptridpai'] ) && $post['refid'] == 'f' ){
			#exclui as inciativas vinculadas ao plano de trabalho
			$obPTA->excluirPtiniciativabeneficiarioDAO( $ptiid );
		}else{*/
			foreach( $dados as $benid ){
				$rural	= $post['rural_'.$benid['codigo']];
				$rural	= $rural=='' ? 0 : (integer) $rural;
				$urbana	= $post['urbana_'.$benid['codigo']];
				$urbana	= $urbana=='' ? 0 : (integer)$urbana;
				$icbid	= $post['icbid_'.$benid['codigo']];
				if( !empty($icbid) )
					$ptbid = $obPTA->inserePtiniciativabeneficiarioDAO( $ptiid, $icbid, $rural, $urbana );
			}
		//}
		if( $this->commit() ){
			if( $boReformulacao ){
				echo "<script>window.opener.location = 'emenda.php?modulo=principal/reformulacaoPTA&acao=A&ptrid=".$post["ptrid"]."'; </script>";
				$this->sucesso('principal/insereBeneficiarioReformulacaoPTA');
			}else
				$this->sucesso('principal/insereBeneficiario');
		} else {
			$this->insucesso('Falha na operação!');
		}
	}
	
	/**
	 * Função carrega as inciativas vinculadas ao plano de trabalho
	 *
	 * @param integer $iniid
	 * @return array
	 */
	public function carregaIniciativaPTA( $iniid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		return $obPTA->carregaIniciativaPTADAO( '', $iniid );
	}
	
	/**
	 * Função carrega o ultimo beneficiarios cadastrado
	 *
	 * @param integer $ptiid
	 * @return max
	 */
	public function carregaBeneficiarioIniciativaPTA( $ptiid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		return $obPTA->carregaBeneficiarioIniciativaPTADAO( $ptiid );
	}
	
	/**
	 * Função monta a listagem com os totais de beneficiarios cadastrados
	 *
	 * @param integer $ptiid
	 * @param integer $iniid
	 * @return html
	 */
	public function mostraAcaoBeneficiarios( $ptiid, $iniid, $ptrid ){
		$obPTA = planoTrabalhoDAO::getInstance();
		
		//$conta = $obPTA->carregaTotalBeneficiarioPTADAO( $ptiid );

		//if( $conta > 0 ){			
			$sql = $obPTA->montaSQLBeneficiarioPTADAO( $ptiid, $ptrid, $iniid );
		/*}
		else {
			$sql = $obPTA->montaSQLBeneficiarioIniciativaPTADAO( $ptiid, $iniid );
		}*/
		
		$dados = $this->carregar($sql);	
		$dados = $dados ? $dados : array();
		
		$html = '
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<thead>
			<tr>
				<td width="40%" align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
					<strong>Beneficiário</strong></td>
				<td width="20%" align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
					<strong>Zona Rural</strong></td>
				<td width="20%" align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
					<strong>Zona Urbana</strong></td>
				<td width="20%" align="Center" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
					<strong>Total</strong></td>
			</tr>
		</thead>';
		
		foreach( $dados as $benef){
			$rural 	= "rural_".$benef['benid'];
			$urbana = "urbana_".$benef['benid'];
			$icbid 	= "icbid_".$benef['benid'];
			$obrigatorio = (($benef['obrigatorio']== t)?'S':'N');
			
			$html.= '
				<tr>
				<td>
					<b>
					<input type="hidden" name="benid" value="'.$benef['benid'].'">
					<input type="hidden" id="'.$icbid.'" name="'.$icbid.'" value="'.$benef['icbid'].'">
					<input type="hidden" id="ptbid" name="ptbid" value="'.$benef['ptbid'].'">
					'.$benef['bennome'].'
					</b>'.
					($benef['benid'] == BENEFICIARIO_ESCOLAS ? "<span onclick=\"popupCadastoEscolas('')\" style=\"float:right;margin-left:50px;cursor:pointer\"><img style=\"vertical-align:middle\" src=\"../imagens/consultar.gif\" title=\"Clique Aqui Para Visualizar Escolas\" /> Visualizar Escolas</span>" : "")
					.'
				</td>';
			if($benef['benid'] == BENEFICIARIO_ESCOLAS || $benef['benid'] == BENEFICIARIO_ALUNOS){
				
				$sql = "select
							count(esc.entid) as num_escolas,
							sum(esc.esbquantidadealunos) as qtde_alunos,
							tpl.tpldesc as zona
						from
							emenda.ptescolasbeneficiadas esc
						inner join
							entidade.entidade ent ON ent.entid = esc.entid
						inner join
							entidade.tipolocalizacao tpl ON tpl.tplid = ent.tplid
						where
							ptrid = {$_SESSION['emenda']['ptrid']}
						group by
							tpl.tpldesc";
				
				$arrDados = $this->carregar($sql);
				$arrDados =!$arrDados ? array() : $arrDados;				
				foreach($arrDados as $dado){
					if($benef['benid'] == BENEFICIARIO_ESCOLAS && $dado['zona'] == "Rural"){
						$benef['rural'] = $dado['num_escolas'];
					}elseif($benef['benid'] == BENEFICIARIO_ESCOLAS && $dado['zona'] == "Urbana"){
						$benef['urbana'] = $dado['num_escolas'];
					}elseif($benef['benid'] == BENEFICIARIO_ALUNOS && $dado['zona'] == "Rural"){
						$benef['rural'] = $dado['qtde_alunos'];
					}elseif($benef['benid'] == BENEFICIARIO_ALUNOS && $dado['zona'] == "Urbana"){
						$benef['urbana'] = $dado['qtde_alunos'];
					}
					
				}
				$benef['total'] = $benef['urbana'] + $benef['rural'];
				
				$html .='<td align="center"><input id="'.$rural.'" class="normal'.( $obrigatorio == 'S' ? ' obrigatorio' : '' ).'" type="text" title="" onblur="MouseBlur(this);" 
							readOnly="readOnly" onclick="popupCadastoEscolas(2)" onmouseout="MouseOut(this);" onkeypress="" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
							onchange="this.value=mascaraglobal(\'[#]\',this.value); CalculaBeneficiario('.$benef['benid'].');" value="'.$benef['rural'].'" maxlength="10" size="9" name="'.$rural.'"/>'.
						
						( $obrigatorio == 'S' ? '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>' : '' )
						.'</td>
					<td align="center"><input id="'.$urbana.'" class="normal'.( $obrigatorio == 'S' ? ' obrigatorio' : '' ).'" type="text" title="" onblur="MouseBlur(this);" 
							readOnly="readOnly" onclick="popupCadastoEscolas(1)" onmouseout="MouseOut(this);" onkeypress="" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
							onchange="this.value=mascaraglobal(\'[#]\',this.value); CalculaBeneficiario('.$benef['benid'].');" value="'.$benef['urbana'].'" maxlength="10" size="9" name="'.$urbana.'"/>'.
						
					( $obrigatorio == 'S' ? '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>' : '' )
					.'</td>';
			}else{
				$html .='<td align="center"><input id="'.$rural.'" class="normal'.( $obrigatorio == 'S' ? ' obrigatorio' : '' ).'" type="text" title="" onblur="MouseBlur(this);" 
							onmouseout="MouseOut(this);" onkeypress="" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
							onkeyup="this.value=mascaraglobal(\'[#]\',this.value); CalculaBeneficiario('.$benef['benid'].');" value="'.$benef['rural'].'" maxlength="10" size="9" name="'.$rural.'"/>'.
						
						( $obrigatorio == 'S' ? '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>' : '' )
						.'</td>
					<td align="center"><input id="'.$urbana.'" class="normal'.( $obrigatorio == 'S' ? ' obrigatorio' : '' ).'" type="text" title="" onblur="MouseBlur(this);" 
							onmouseout="MouseOut(this);" onkeypress="" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
							onkeyup="this.value=mascaraglobal(\'[#]\',this.value); CalculaBeneficiario('.$benef['benid'].');" value="'.$benef['urbana'].'" maxlength="10" size="9" name="'.$urbana.'"/>'.
						
					( $obrigatorio == 'S' ? '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>' : '' )
					.'</td>';
			}	
			$html .='<td align="center">
					<div id="total_'.$benef['benid'].'">
						<b>'.$benef['total'].'</b>
					</div>
				</td>
			</tr>';
		}			
		/*if($conta>0){
			$dados = $obPTA->carregaTotaisBeneficiarios( $ptiid );
			$dados = $dados ? $dados : array();
			foreach( $dados as $benef){
			$html.= "	<tr>
						<td><b>".$benef['titulo']."</b></td>
						<td align='center'><div id='total_rural'><b>".$benef['total_rural']."</b></div></td>
						<td align='center'><div id='total_urbana'><b>".$benef['total_urbana']."</b></div></td>
			 			<td align='center'><div id='total_geral'><b>".$benef['total_geral']."</b></div></td>
					</tr>";
			}
		}
		else {
			$html.= "	<tr>
						<td><b>TOTAL</b></td>
						<td align='center'><div id='total_rural'><b></b></div></td>
						<td align='center'><div id='total_urbana'><b></b></div></td>
			 			<td align='center'><div id='total_geral'><b></b></div></td>
					</tr>";
		}*/
		$html.= "</table>";
		
		return $html;
	}

	public function listaEscolaBeneficiada( $ptrid ){
		$obPTA = planoTrabalhoDAO::getInstance();
				
		$dados = $obPTA->listaEscolaBeneficiadaDAO( $ptrid );	
		$dados = $dados ? $dados : array();
		
		if(!verificaPermissaoPerfil('pta', 'boolean', $this->pegarEstadoAtual( $ptrid ), true) ){
			$incluir = '<img style="cursor:pointer" name="btSalvar" src="../imagens/gif_inclui_d.gif" id="btSalvar" title="Incluir Escola"  /> 
						<img style="cursor:pointer" name="btCancelar" src="../imagens/excluir_01.gif" id="btCancelar" title="Cancelar" />';
		} else {
			$incluir = '<img style="cursor:pointer" name="btSalvar" src="../imagens/gif_inclui.gif" id="btSalvar" title="Incluir Escola" onclick=\'buscaEscola(document.formulario.inepID.value); salvarEscola();\' /> 
						<img style="cursor:pointer" name="btCancelar" src="../imagens/excluir.gif" id="btCancelar" title="Cancelar" onclick=\'CancelaEscola();\' />';
		}
		
		$html = '<input type="hidden" name="ptrid" id="ptrid" value="'.$ptrid.'">
				<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td width="20%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Código Censo Escolar - INEP</strong></td>
						<td width="50%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Nome da Escola</strong></td>
						<td align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Zona</strong></td>
						<td width="10%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Alunos<br />Beneficiados</strong></td>
						<td width="20%" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">
							<strong>Ação</strong></td>
					</tr>
				</thead>
					<tr>
						<td>
							<input type="hidden" name="esbid" id="esbid" value="">
							<div style="white-space:nowrap">
							<a href="#" onclick="consultarINEP()"><img style="vertical-align:middle" src="../imagens/consultar.gif" title="Consultar código INEP" style="cursor:pointer;" border="0"></a>
							<input size="10" class="normal" type="text" name="inepID" size="18" id="inepID" value="" style="text-align:center" onkeypress="return somenteNumeros(event);">
							</div>
						</td>
						<td id="td_escola" align="Center" >
							<input type="hidden" name="entid" id="entid" value="">
							<input class="normal" type="text" size="100" name="nmescola" id="nmescola" readonly style="text-align:center" value="" onfocus="buscaEscola(document.formulario.inepID.value);">
						</td>
						<td id="td_zona" align="Center" >
						</td>
						<td><input size="10" class="normal" type="text" name="esbquantidadealunos" id="esbquantidadealunos" style="text-align:center" value="" onkeypress="return somenteNumeros(event);" onfocus="buscaEscola(document.formulario.inepID.value)"></td>
						<td align="Center">
							<div style="white-space:nowrap">
								'.$incluir.'
							</div>
						</td>
					</tr>
					<tr>';

		foreach( $dados as $esb ){
		  if($this->possuiPermissao($this->pegarEstadoAtual( $ptrid ) ) ) {
			$acao = "<a style=\"cursor:pointer;\" onclick=\"alterar($esb[esbid]);\">
						<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
					</a>
					<a  style=\"cursor:pointer;\" onclick=\"removerEscola($esb[esbid]);\">
						<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
					</a>";
		  } else {
		  	$acao = "<img src=\"/imagens/alterar_01.gif \" border=0 title=\"Alterar\"> 				
						<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
		  }
			
			 $html.= "	<tr>
						<td align='center'><b>
								<input type='hidden' name='esbid_".$esb['esbid']."' id='esbid_".$esb['esbid']."' value='".$esb['esbid']."'>
								<input type='hidden' name='entid_".$esb['esbid']."' id='entid_".$esb['esbid']."' value='".$esb['entid']."'>
								<input type='hidden' name='inepID_".$esb['esbid']."' id='inepID_".$esb['esbid']."' value='".$esb['entcodent']."'>
								<input type='hidden' name='entnome_".$esb['esbid']."' id='entnome_".$esb['esbid']."' value='".$esb['entnome']."'>
								<input type='hidden' name='esbquantidadealunos_".$esb['esbid']."' id='esbquantidadealunos_".$esb['esbid']."' value='".$esb['esbquantidadealunos']."'>".
			 					$esb['entcodent']."</b></td>
						<td id=\"td_estnome_".$esb['esbid']."\" align='center'>".$esb['entnome']."</td>
						<td id=\"td_zona_".$esb['esbid']."\" align='center'>".$esb['tpldesc']."</td>
						<td id=\"td_qtde_alunos_".$esb['esbid']."\" align='center'>".$esb['esbquantidadealunos']."</td>
						<td align='center'>".$acao."</td>
					</tr>";	
		}
		$sql = "SELECT count(*) as quant,sum(esbquantidadealunos) as total
				FROM
					emenda.ptescolasbeneficiadas peb
				    inner join entidade.entidade ent
				    	on ent.entid = peb.entid
				    inner join entidade.funcaoentidade fent
				        ON fent.entid = ent.entid
				    inner join entidade.tipolocalizacao tpl
				    	ON ent.tplid = tpl.tplid
				WHERE
					ent.entstatus = 'A'
				    and fent.funid in(3,4,11,12,16,18,56) 
				    and peb.ptrid = $ptrid";
				
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		foreach( $dados as $esb ){
			if($esb['quant']>1)
				$rotulo = " Escolas";
			else
				$rotulo = " Escola"; 
			$html.= "	<tr bgcolor='#c0c0c0'>
						<td align='center'><b> Totais</b></td>
						<td colspan=2 align='center'><b>".$esb['quant'].$rotulo."</b></td>
						<td align='center'><b>".$esb['total']."</b></td>
						<td align='center'></td>
					</tr>";	
		}
		$html.= "</table>";
		
		echo $html;
	}
}
?>