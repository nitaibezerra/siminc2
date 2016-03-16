<?php
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
	
class Processo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela 				= "execucaofinanceira.processos";	
    public $tipoExecucao 					= NULL;
    public $tela 							= NULL; // Tela que esta carregando o processo - pode ser tela de Empenho ou pagamento
    public $inuid 							= NULL;
    public $esfera 							= NULL;
    public $nomeSistema 					= NULL;
    public $sisid 							= NULL;
    public $tipvaiparasigarp 				= NULL;
    public $tipcodigoobservacaosigef		= NULL;
	public $tiptipoempenhosigef				= NULL; 
  	public $tipcodigodescricaoempenhosigef	= NULL;
	public $tipnumerosistemasigef			= NULL;
	public $tipvaiparamonitoramentodeobras	= NULL;
	public $tipprogramafnde					= null;
	public $tiptipoprocessosigef			= null;

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "proid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'proid' 					=> null, 
									  	'pronumeroprocesso' 		=> null,
    									'tprid' 					=> null, 
									  	'procnpj' 					=> null, 
									  	'proesfera' 				=> null, 
									  	'pronumeroconveniosigef' 	=> null, 
									  	'prodatainclusao' 			=> null, 
									  	'prodocumenta' 				=> null, 
									  	'pronumeroconveniosiafi'	=> null, 
									  	'proano' 					=> null, 
									  	'prostatus'					=> null, 
									  	'muncod' 					=> null, 
									  	'estuf' 					=> null, 
									  	'usucpf' 					=> null, 
									  	'tipid' 					=> null,
									  	'provalortermo' 			=> null,
									  	'provalorempenho' 			=> null,
									  	'provalorpagamento'			=> null,
									  );
								  
	function __construct() {
		parent::__construct();
	}
					  
	public function listaprocessos($post = NULL, $arrPerfil = array()){
		
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
			if($post['empenhado'] == "1") 		$where[] = " p.pronumeroprocesso in (select distinct empnumeroprocesso from par.empenho where empnumeroprocesso = p.pronumeroprocesso and empstatus = 'A') ";  //"(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) > 0";
			if($post['empenhado'] == "2") 		$where[] = " p.pronumeroprocesso not in (select distinct empnumeroprocesso from par.empenho where empnumeroprocesso = p.pronumeroprocesso and empstatus = 'A') "; // "(SELECT sum(ep2.empvalorempenho) FROM par.empenho ep2 WHERE ep2.empnumeroprocesso=p.pronumeroprocesso) = 0";
			if( $post['tprid'] )				$where[] = " p.tprid = {$post['tprid']}";
			if( $post['tipid'] )				$where[] = " p.tipid = {$post['tipid']}";
			
			if($post['esfera'] == "e") {
				$where[] = "p.proesfera = 'E' ";
			} 
			if($post['esfera'] == "m") {
				$where[] = "p.proesfera = 'M' ";
			}
	
			if($post['anoprocesso']) $where[] = "p.proano = '".$post['anoprocesso']."'";
		}
		
		if( !in_array( EXEC_PERFIL_ADMINISTRADOR, $arrPerfil ) && !$this->testa_superuser() ){
			$btnAcoes = "<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"visualizarProcesso('|| p.proid ||')\"><img title=\"Visualizar todo o processo.\" style=\"width: 18px; height: 18px;\" src=\"/imagens/livro.gif\" border=0 title=\"Selecionar\"></a> 
						<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"editarProcesso('|| p.proid ||')\"><img title=\"Alterar dados do processo.\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
						<a style=\"margin: 0 -5px 0 5px;\" href=\"#\"><img title=\"Excluir processo.\" src=\"/imagens/excluir_01.gif\" border=0 title=\"Selecionar\"></a>";
		} else {
			$btnAcoes = "<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"visualizarProcesso('|| p.proid ||')\"><img title=\"Visualizar todo o processo.\" style=\"width: 18px; height: 18px;\" src=\"/imagens/livro.gif\" border=0 title=\"Selecionar\"></a> 
						<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"editarProcesso('|| p.proid ||')\"><img title=\"Alterar dados do processo.\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
						<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"excluirProcesso('|| p.proid ||')\"><img title=\"Excluir processo.\" src=\"/imagens/excluir.gif\" border=0 title=\"Selecionar\"></a>";
		}
		
		$where[] = "p.pronumeroprocesso in ('23400003167201267', '23034000045201234')";
		
		$sql = "SELECT distinct 	'<center>
							<img style=\"cursor:pointer\" id=\"img_dimensao_'|| p.proid ||'\" src=\"/imagens/mais.gif\" onclick=\"carregarDadosProcesso(this.id,'|| p.proid ||');\" border=\"0\"> 
							$btnAcoes
						</center>' as acao, 
						p.pronumeroprocesso, 
						p.prodocumenta, 
						p.estuf, 
						p.muncod, 
						m.mundescricao, 
						to_char(p.prodatainclusao, 'DD/MM/YYYY') as data, 
						---(select sum(pc.prcvalortotal) from execucaofinanceira.processocomposicao pc where pc.proid = p.proid) as prcvalortotal,  
						tp.tipdescricao ||
							'</td></tr>
				            	<tr style=\"display:none\" id=\"listaDadosProcesso_' || p.proid || '\" >
				            		<td id=\"trV_' || p.proid || '\" colspan=8 ></td>
				            </tr>' as descricao
				FROM execucaofinanceira.processos p
					inner join execucaofinanceira.tipoprocesso tp on tp.tipid = p.tipid
					left join territorios.municipio m ON m.muncod = p.muncod
				".($where ? " WHERE ".implode(" and ", $where) : '')." order by m.mundescricao";
		//ver( simec_htmlentities($sql),d);
		$cabecalho = Array('Ação','Numero do Processo','Nº no sistema Documenta','UF','Código IBGE', 'Município', 'Data de Inclusão', 'Valor', 'Processo do Tipo');
		return $this->monta_lista( $sql, $cabecalho, 50, 20, 'N', 'center','N');
	}
	
	/*public function controleDeTela($tela){
		// ------------- REGRAS DE NEGÓCIO PARA CARREGAMENTO DE TELA (CONTROLE DE FLUXO) ------------- //
		$this->tela 							= $tela;			
		$arrTipos 								= $this->getTipoProcesso();
		$this->tipvaiparasigarp 				= (boolean)$arrTipos['tipvaiparasigarp'];
		$this->tipcodigoobservacaosigef 		= $arrTipos['tipcodigoobservacaosigef'];
  		$this->tiptipoempenhosigef 				= $arrTipos['tiptipoempenhosigef'];
  		$this->tipcodigodescricaoempenhosigef	= $arrTipos['tipcodigodescricaoempenhosigef'];
  		$this->tipnumerosistemasigef 			= $arrTipos['tipnumerosistemasigef'];
  		$this->tipvaiparamonitoramentodeobras	= (boolean)$arrTipos['tipvaiparamonitoramentodeobras'];
  		$this->tipprogramafnde					= $arrTipos['tipprogramafnde'];
  		$this->tiptipoprocessosigef				= $arrTipos['tiptipoprocessosigef'];
  					
		// (EXCLUSIVO PARA O PAR) Carrega id do PAR caso seja processos de empenho e pagamento de subaçãoes, obras do PAR
		if($this->tipid == SUBACOES_GENERICO_PAR || $this->tipid == SUBACOES_PAR_EMENDAS ){
			$this->inuid = $this->pegaUm("SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$this->muncod."'");
	    	if(!$this->inuid){
	    		$this->inuid = $this->pegaUm("SELECT inuid FROM par.instrumentounidade WHERE estuf = '".$this->estuf."' AND muncod is null");
	    	}	    	
	    	$this->nomeSistema == 'PAR';	    	
		}
	}*/
	
	public function getTipoProcesso(){
		$sql = "SELECT 
					tipdescricao,
					tipstatus,
				  	sisid,
				  	tpdid,
				  	tipvaiparasigarp,
				  	tprid,
				  	tipcodigoobservacaosigef,
				  	tiptipoempenhosigef,
				  	tipcodigodescricaoempenhosigef,
				  	tipnumerosistemasigef,
				  	tipvaiparamonitoramentodeobras,
				  	tipprogramafnde,
				  	tiptipoprocessosigef
				FROM 
				  	execucaofinanceira.tipoprocesso
				WHERE tipid = {$this->tipid} and tipstatus = 'A'";
				
		$arrTipos = $this->pegaLinha($sql);
		return $arrTipos;
	}
	
	public function montaCabecalhoProcesso(){
		
		$tipdescricao = $this->pegaUm("SELECT tipdescricao FROM execucaofinanceira.tipoprocesso WHERE tipid = {$this->tipid} and tipstatus = 'A'");
		
		if( trim($this->proesfera) == 'M' ){
			$label = 'Município';
			$municipio = $this->pegaUm("select mundescricao from territorios.municipio where muncod = '{$this->muncod}'");
		} else {
			$label = 'Estado';
			$municipio = $this->pegaUm("select estdescricao from territorios.estado where estuf = '{$this->estuf}'");
		}
		$valorProcesso = $this->pegaUm("SELECT sum(prcvalortotal) FROM execucaofinanceira.processocomposicao WHERE proid = {$this->proid}");
		$html = '<table class="tabela" id="cabecalhoempenho" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<tbody>
					<tr>
						<td colspan="6" class="subtitulocentro"><b>Informações do Processo</b></td>
					</tr>
					<tr>
						<td class="subtituloDireita" style="width: 15%"><b>Processo:</b></td>
						<td style="width: 18%">'.$this->mascaraglobal($this->pronumeroprocesso, '#####.######/####-##').'</td>
						<td class="subtituloDireita" style="width: 15%"><b>CNPJ:</b></td>
						<td style="width: 18%">'.$this->mascaraglobal($this->procnpj, '##.###.###/####-##').'</td>
						<td class="subtituloDireita" style="width: 15%"><b>Valor do Termo:</b></td>
						<td style="width: 18%">R$ '.number_format(0, 2, ',', '.').'</td>
					</tr>
					<tr>
						<td class="subtituloDireita" style="width: 15%"><b>Ano do Processo:</b></td>
						<td style="width: 18%">'.$this->proano.'</td>						
						<td class="subtituloDireita" style="width: 15%"><b>Entidade:</b></td>
						<td style="width: 18%">'.$this->pegaUm("select entnome from entidade.entidade where entnumcpfcnpj = '{$this->procnpj}'").'</td>
						<td class="subtituloDireita" style="width: 15%"><b>Valor do Empenho:</b></td>
						<td style="width: 18%">R$ '.number_format(0, 2, ',', '.').'</td>
					</tr>
					<tr>
						<td class="subtituloDireita" style="width: 15%"><b>Tipo:</b></td>
						<td style="width: 18%">'.$tipdescricao.'</td>
						<td class="subtituloDireita" style="width: 15%"><b>'.$label.':</b></td>
						<td style="width: 18%">'.$municipio.'</td>
						<td class="subtituloDireita" style="width: 15%"><b>Valor do Pagamento:</b></td>
						<td style="width: 18%">R$ '.number_format(0, 2, ',', '.').'</td>							
					</tr>
					<tr>
						<td class="subtituloDireita" style="width: 15%"><b>Valor do Processo:</b></td>
						<td style="width: 18%">R$ '.number_format($valorProcesso, 2, ',', '.').'</td>
						<td class="subtituloDireita" style="width: 15%"><b>UF:</b></td>
						<td style="width: 18%">'.$this->estuf.'</td>
						
						<td class="subtituloDireita" style="width: 15%"></td>
						<td style="width: 18%"></td>
					</tr>
				</tbody>
				</table>';
		
		echo $html;
	}
	
	public function carregaListaDadosProcesso(){
		
		switch ($this->tipid) {
			case SUBACOES_GENERICO_PAR:
				echo '<br>';
				monta_titulo('', '<b>Lista de Subações Vinculadas ao Processo<b>');
				$obSubGenPAR = new SubacaoGenericoPar($this);
				$sql = $obSubGenPAR->carregaListaDadosProcesso();
			break;
			case OBRAS_PAR:
				echo '<br>';
				monta_titulo('', '<b>Lista de Subações Vinculadas ao Processo<b>');
				$obObrasPar = new ObrasPar($this);
				$sql = $obObrasPar->carregaListaDadosProcesso();
			break;
			case OBRAS_PAC:
				echo '<br>';
				monta_titulo('', '<b>Lista de Subações Vinculadas ao Processo<b>');
				$obObrasPac = new ObrasPac($this);
				$sql = $obObrasPac->carregaListaDadosProcesso();
			break;
		}
		
		$cabecalho = array("Descrição", "Ano", "Valor");
		$this->monta_lista_simples($sql, $cabecalho, 500, 5, 'S', '95%', 'S');		
	}
	
	public function carregaInformacoesProcesso( $proid ){
		
		$sql = "SELECT DISTINCT
		            e.empnumero,
		            e.empvalorempenhado as valorempenhado,
		            cast(((sum(DISTINCT ec.emcvaloremp) * 100) / sum( DISTINCT pc.prcvalortotal)) as numeric(20)) AS porcentagem,
                    to_char(ec.emcdata, 'DD/MM/YYYY') as data,
                    us.usunome
				FROM execucaofinanceira.processos p
					INNER JOIN execucaofinanceira.empenho e ON p.proid = e.proid 
					INNER JOIN execucaofinanceira.empenhocomposicao ec ON ec.empid = e.empid
					INNER JOIN execucaofinanceira.processocomposicao pc ON pc.proid = p.proid
                    inner join seguranca.usuario us on us.usucpf = ec.usucpf
				WHERE p.prostatus = 'A'
					AND e.empstatus = 'A'
					AND p.proid = {$proid}
				GROUP BY 
		             e.empnumero,
		             e.empvalorempenhado,
		             ec.emcvaloremp,
                     ec.emcdata,
                     us.usunome";    
        
        /*$cabecalho = array('Número', 'Valor', '(%) Empenhada', 'Data', 'Solicitante');
        $this->monta_lista_simples($sql, $cabecalho, 500, 5, 'S', '50%', 'S');*/
        
        $arrEmpenho = $this->carregar($sql);
        if( $arrEmpenho ){
        	$htmlEmpenho = '<table width="100%" cellspacing="0" cellpadding="2" border="0" align="left" class="listagem" style="color:333333;">
					<thead>
					<tr>
						<th colspan="5">Dados do Empenho</th>
					</tr>
					<tr>
						<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Número</td>
						<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Valor</td>
						<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">(%) Empenhada</td>
						<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Data</td>
						<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Solicitante</td>
					</tr> 
					</thead>
					<tbody>';
        	
        	$totalValor = 0;
        	$totalPorce = 0;
	        foreach ($arrEmpenho as $key => $v) {
	        	$totalValor += (float)$v['valorempenhado'];
	        	$totalPorce += (float)$v['porcentagem'];
	        	
	        	$key % 2 ? $cor = "#F7F7F7" : $cor = "";
	        	$htmlEmpenho.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'"onmouseout="this.bgColor=\''.$cor.'\';"onmouseover="this.bgColor=\'#ffffcc\';">
							<td valign="top" title="Número">'.$v['empnumero'].'</td>
							<td valign="top" align="right" title="Valor" style="color:#999999;">'.number_format($v['valorempenhado'], 2, ',', '.').'</td>
							<td valign="top" align="right" title="(%) Empenhada" style="color:#999999;">'.$v['porcentagem'].'</td>
							<td valign="top" title="Data">'.$v['data'].'</td>
							<td valign="top" title="Solicitante">'.$v['usunome'].'</td>
						</tr>';
	        }
	        $htmlEmpenho.= '</tbody>
					<tfoot>
					<tr>
						<td align="right" title="Número">Totais:   </td>
						<td align="right" title="Valor">'.number_format($totalPorce, 2, ',', '.').'</td>
						<td align="right" title="(%) Empenhada">'.$totalPorce.'</td>
						<td align="right" title="Data"></td>
						<td align="right" title="Solicitante"></td>
					</tr>
					</tfoot>
					</table>';
        } else {
        	
        }		
		
		$html = '
			<table style="width: 50%" align="left" cellspacing="1" cellpadding="3">
			<tr>
				<td align="right" width="10%" valign="top"><img style="cursor:pointer" src="/imagens/seta_filho.gif" border="0"></td>
				<td>
					'.$htmlEmpenho.'
				</td>
			</tr>
			<tr>
				<td align="right" width="10%" valign="top"><img style="cursor:pointer" src="/imagens/seta_filho.gif" border="0"></td>
				<td>
					<table class="tabela" id="cabecalhoempenho" align="left" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
						<thead>
							<tr>
								<td class="subtituloCentro" colspan="4">PAGAMENTO</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<th>Número</th>
								<th>Data</th>
								<th>Solicitante</th>
							</tr>
						</tbody>
					</table>
				</td>
			</tr>
			<tr>
				<td align="right" width="10%" valign="top"><img style="cursor:pointer" src="/imagens/seta_filho.gif" border="0"></td>
				<td>
					<table class="tabela" id="cabecalhoempenho" align="left" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
						<thead>
							<tr>
								<td class="subtituloCentro" colspan="4">DOCUMENTO</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<th>Número</th>
								<th>Data</th>
								<th>Solicitante</th>
							</tr>
						</tbody>
					</table>
				</td>
			</tr>
			</table>';
		
		echo $html;
	}
	
	public function mascaraglobal($value, $mask) {
      $casasdec = explode(",", $mask);
      // Se possui casas decimais
      if($casasdec[1])
            $value = sprintf("%01.".strlen($casasdec[1])."f", $value);

      $value = str_replace(array("."),array(""),$value);
      if(strlen($mask)>0) {
            $masklen = -1;
            $valuelen = -1;
            while($masklen>=-strlen($mask)) {
                  if(-strlen($value)<=$valuelen) {
                        if(substr($mask,$masklen,1) == "#") {
                                   $valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
                                   $valuelen--;
                        } else {
                             if(trim(substr($value,$valuelen,1)) != "") {
                                   $valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
                             }
                        }
                  }
                  $masklen--;
            }
      }
      return $valueformatado;
	}
	
	public function vincularProcesso($dados) {
		
    	$an_processo 		= date("Y");
    	$nu_processo		= $this->pronumeroprocesso;
    	$tp_processo 		= $this->tiptipoprocessosigef;    	
    	$co_programa_fnde	= $this->tipprogramafnde;
		$nu_cnpj_favorecido = $this->procnpj;	
		$data_created 		= date("c");
		$usuario 			= $dados['wsusuario'];
		$senha   			= $dados['wssenha'];		
	
	    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<params>
      	<nu_cnpj>$nu_cnpj_favorecido</nu_cnpj>
      	<nu_processo>$nu_processo</nu_processo>
      	<tp_processo>$tp_processo</tp_processo>
      	<an_processo>$an_processo</an_processo>
      	<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;


		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));
	
		$result = (integer) $xml->status->result;
		if(!$result) {
			
			$mensagem = '';
			$erros = $xml->status->error->message;
			
			if(count($erros)>0) {	
				foreach($erros as $err) {
			 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}
			echo $this->mensagem( 'ERRO SOLICITAÇÃO DE PROCESSO',  $mensagem);
			
			$arrHistorico = array('tipo' => 'vincularProcesso - Erro',
								  'proid' => $this->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$this->insereHistoricoWS($arrHistorico);

		    return false;
		} else {			
			$arrHistorico = array('tipo' => 'vincularProcesso - Sucesso',
								  'proid' => $this->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$this->insereHistoricoWS($arrHistorico);

			return true;
		}
	}
	
	public function solicitarConvenio($dados){	
	
		$usuario = $dados['wsusuario'];
		$senha   = $dados['wssenha'];
		$xmlRetorno = "";
	
		$nu_cnpj_favorecido = $this->procnpj;
		$co_programa_fnde	= $this->tipprogramafnde;	
		$anoAtual     		= date('Y');
		$dataAtualXml 		= date("c");
		
		if($nu_cnpj_favorecido){
				$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>$dataAtualXml</created>
</header>
	<body>
	<params>
		<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		<an_exercicio>$anoAtual</an_exercicio>
	</params>
	</body>
</request>
XML;

			if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/convenio/consultar";						
			} else {
				$urlWS = "http://www.fnde.gov.br/webservices/sigefemendas/index.php/convenio/consultar";
			}
			try {
	
				
		    	$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
					->execute();
			
				$xmlRetorno = $xml;
				$xml = simplexml_load_string( stripslashes($xml));
	
				if ( (int) $xml->status->result ){
			        $obConvenio = $xml->body->children();
	
			        $nu_convenio  = (int) $obConvenio->nu_convenio;
			        $an_exercicio = (int) $obConvenio->an_exercicio;
	
					if($nu_convenio && $an_exercicio){
						# Atualiza a tabela de processopar.
						
						$sql = "UPDATE execucaofinanceira.processos SET pronumeroconveniosigef = {$nu_convenio} WHERE proid = {$this->proid}";
						$this->executar($sql);
						
						$arrHistorico = array('tipo' => 'solicitarConvenio - Sucesso',
											  'proid' => $this->proid,
											  'envio' => $arqXml,
											  'retorno' => $xmlRetorno
											);
						$this->insereHistoricoWS($arrHistorico);
				
					} else {
						$mensagem = '';
						$erros = $xml->status->error->message;
						
						if(count($erros)>0) {	
							foreach($erros as $err) {
						 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
							}
						}
						$arrHistorico = array('tipo' => 'solicitarConvenio - Erro',
											  'proid' => $this->proid,
											  'envio' => $arqXml,
											  'retorno' => $xmlRetorno
											);
						$this->insereHistoricoWS($arrHistorico);
						
						echo $this->mensagem( 'ERRO CONSULTAR NÚMERO DE CONVÊNIO',  $mensagem);
					}
			    } else {
			    	$mensagem = '';
					$erros = $xml->status->error->message;
					
					if(count($erros)>0) {	
						foreach($erros as $err) {
					 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
						}
					}
					$arrHistorico = array('tipo' => 'solicitarConvenio - Erro',
										  'proid' => $this->proid,
										  'envio' => $arqXml,
										  'retorno' => $xmlRetorno
										);
					$this->insereHistoricoWS($arrHistorico);
						
					echo $this->mensagem( 'ERRO CONSULTAR NÚMERO DE CONVÊNIO',  $mensagem);
			    }
	
			} catch (Exception $e){
				/**
				 * @var LogErroWS
				 * Bloco que grava o erro em nossa base
				 */
				$arrHistorico = array('tipo' => 'solicitarConvenio - Erro',
									  'proid' => $this->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$this->insereHistoricoWS($arrHistorico);
								
				# Erro 404 página not found
				if($e->getCode() == 404){
					$$mensagem = "Erro-Serviço Convênio encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
					echo $this->mensagem( 'ERRO CONSULTAR NÚMERO DE CONVÊNIO',  $mensagem);
					return false;
					die;
				}
				
				$$mensagem = 'Erro-WS Consulta Convenio: '.$e->getMessage();
				echo $this->mensagem( 'ERRO CONSULTAR NÚMERO DE CONVÊNIO',  $mensagem);
				return false;
				die;
			}	
		}
		//$db->commit();
		return $obConvenio;
	}
	
	public function mensagem( $title, $msg ){
		$html = '<div style=" border: 1px solid #B7B7B7; font-size: 11px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					'.$title.'
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >'.$msg.'</div>
				</div>
			 	<br>';
		return $html;
	}

	public function consultaHabilitaEntidade($cnpj){
		$xmlRetorno = "";
		if(!$cnpj){
			echo 'Erro-CNPJ Inexistente.';
			die;
		}
		$dataAtualXml = date("c");

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?><request><header><app>string</app><version>string</version><created>{$dataAtualXml}</created>
</header>
    <body>
		<params>
			<cnpj>$cnpj</cnpj>
		</params>
    </body>
</request>
XML;

		$xml = Fnde_Webservice_Client::CreateRequest()
			//->setURL('http://172.20.200.116/webservices/habilita/base/web/dev.php/entidade/consultar')
			->setURL('http://www.fnde.gov.br/webservices/habilita/index.php/entidade/consultar')
			->setParams( array( 'xml' => $arqXml ) )
			->execute()
		;
		$xmlRetorno = $xml;
		$xml = simplexml_load_string( stripslashes($xml));

		if ( (int) $xml->status->result ){
	        $obHabilita = $xml->body->row->children();
	        $co_situacao_habilita = (int) $obHabilita->co_situacao_habilita;
			$ds_situacao_habilita = (string) $obHabilita->ds_situacao_habilita;
			$arrRetorno = array("codigo" => $co_situacao_habilita, "descricao" => iconv( "UTF-8", "ISO-8859-1", $ds_situacao_habilita));
			
			$arrHistorico = array('tipo' => 'consultaHabilitaEntidade - Sucesso',
								  'proid' => $this->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$this->insereHistoricoWS($arrHistorico);
			
			return $arrRetorno;
	       	/*if($co_situacao_habilita == 3){
	       		return iconv( "UTF-8", "ISO-8859-1", $ds_situacao_habilita);
	       	} else {
	       		$ds_situacao_habilita = ( $ds_situacao_habilita ? $ds_situacao_habilita : '1'  );
	       		return $ds_situacao_habilita;		        		
	       	}*/
	    } else {
	    	$arrHistorico = array('tipo' => 'consultaHabilitaEntidade - Erro',
								  'proid' => $this->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$this->insereHistoricoWS($arrHistorico);
			return 1;
	    }
	}
	
	public function insereHistoricoWS($arrHistorico = array()){
		$sql = "INSERT INTO execucaofinanceira.historicows(hwsdescricaowebservice, proid, hwsxmlenvio, hwsxmlretorno, hwsdataenvio, usucpf) 
				VALUES ('".$arrHistorico['tipo']."',
  						'".$arrHistorico['proid']."',
  						'".addslashes($arrHistorico['envio'])."',
			    		'".addslashes($arrHistorico['retorno'])."',
			    		NOW(),
			            '".$_SESSION['usucpf']."')";

		$this->executar($sql);
		return $this->commit();
	}

}