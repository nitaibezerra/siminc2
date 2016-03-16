<?php
class Processos extends cls_banco{
	private $arCampos;
	private $arFiltros;
	
	public function __construct(){
		parent::__construct();
		$this->arFiltros = '';
		$this->arCampos = array( 'prcid', 'cesid', 'secid', 'tpcid', 'dreid', 'sitid', 'intid', 'tilid', 'gerid', 'prcnumprocesso',
						   		 'prcnumdocumento', 'prcdataentradaccon', 'prcobjeto', 'prcnumportaria', 'prcretificacao', 'prcvalor',
						   		 'prcdatainiciovigencia', 'prcdatafimvigencia', 'prcnumconveniosiafi', 'prccedente', 'prcdou', 'prcstatus');
		
	}
	
	public function cadastraProcessos( $post ){
		if( !empty($post['prcdatainiciovigencia']) && !empty($post['prcdatafimvigencia']) ){
			$post['prcdatainiciovigencia'] = formata_data_sql( $post['prcdatainiciovigencia'] );
			$post['prcdatafimvigencia'] = formata_data_sql( $post['prcdatafimvigencia'] );
		}
		if( !empty($post['prcvalor']) ){
			$post['prcvalor'] = $this->retiraPontos($post['prcvalor']);
		}
		if( !empty( $post['prcdataentradaccon'] ) ){
			$post['prcdataentradaccon'] = formata_data_sql( $post['prcdataentradaccon'] );
		}
		
		if( !empty( $post['prcnumconvenio'] ) ){
			$post['prcnumdocumento'] = $post['prcnumconvenio'];
		}
		if( !empty( $post['prcnumcontrato'] ) ){
			$post['prcnumdocumento'] = $post['prcnumcontrato'];
		}
		if( !empty( $post['prcnumdispensa'] ) ){
			$post['prcnumdocumento'] = $post['prcnumdispensa'];
		}
		if( !empty( $post['prcnuminegixibilidade'] ) ){
			$post['prcnumdocumento'] = $post['prcnuminegixibilidade'];
		}
		if( !empty( $post['prcnumacordo'] ) ){
			$post['prcnumdocumento'] = $post['prcnumacordo'];
		}
		if( !empty( $post['prcnumdoacao'] ) ){
			$post['prcnumdocumento'] = $post['prcnumdoacao'];
		}
		
		$post['prcnumprocesso'] = str_replace( '.', '', $post['prcnumprocesso'] );
		$post['prcnumprocesso'] = str_replace( '/', '', $post['prcnumprocesso'] );
		$post['prcnumprocesso'] = str_replace( '-', '', $post['prcnumprocesso'] );
		
		
		$arDados = array();		
		$arCampos = array();
		$arValor = array();
		
		if( $post['prcid'] ){			
			foreach( $this->arCampos as $campo ){			
				if( key_exists( $campo, $post ) ){
					if( $post[$campo] !== null){
						if( $campo != 'prcid' ){
							$valor = simec_htmlspecialchars($post[$campo] , ENT_QUOTES);				
							$campos .= $campo." = '".trim($valor)."', ";
						}
					} else {
						$campos .= $campo." = null, ";
					}
				}
			}
			
			$campos = substr( $campos, 0, -2 );
			
			$sql = "UPDATE seed.processo SET $campos WHERE prcid = ".$post['prcid'];
			
		} else {
			foreach( $this->arCampos as $campo ){			
				if( key_exists( $campo, $post ) ){
					if( $campo != 'prcid' ){
						$arCampos[] = $campo;
						$arValor[]  = $post[$campo];
					}
				}
			}
			$sql = "INSERT INTO seed.processo(". implode( ', ', $arCampos ) ." ) 
									   values('". implode( "', '", $arValor ) ."' )";
			
		}
		if( $post['tipo'] == 'cadastrar' ){
			$modulo = 'principal/listaProcessos';
			$parametro = '';
		} else {
			$modulo = 'principal/alterarProcesso';
			$parametro = '&prcid='.$post['prcid'];
		}  
		
		parent::executar( $sql );
		if( parent::commit() ){
			parent::sucesso( $modulo, $parametro );
		} else {
			parent::insucesso( 'Falha na operação' );
		}		
	}
	
	public function excluirProcesso( $prcid ){
		$sql = "UPDATE seed.processo SET prcstatus = 'I' WHERE prcid = $prcid";
		parent::executar( $sql );
		if( parent::commit() ){
			parent::sucesso( 'principal/listaProcessos' );
		} else {
			parent::insucesso( 'Falha na operação' );
		}
	}
	
	public function montaFiltroProcesso( $post ){
		if( !empty( $post['prcnumprocesso'] ) ){
			$post['prcnumprocesso'] = str_replace( '.', '', $post['prcnumprocesso'] );
			$post['prcnumprocesso'] = str_replace( '/', '', $post['prcnumprocesso'] );
			$post['prcnumprocesso'] = str_replace( '-', '', $post['prcnumprocesso'] );
		}
		
		$this->arFiltros .= $post['tpcid'] ? " and p.tpcid = ".$post['tpcid'] : '';
		$this->arFiltros .= $post['prcnumprocesso'] ? " and p.prcnumprocesso = '".$post['prcnumprocesso']."'" : '';
		$this->arFiltros .= $post['prcdataentradaccon'] ? " and p.prcdataentradaccon = '".formata_data_sql( $post['prcdataentradaccon'] )."'" : '';
		$this->arFiltros .= $post['prcdatainiciovigencia'] ? " and p.prcdatainiciovigencia = '".formata_data_sql( $post['prcdatainiciovigencia'] )."'" : '';
		$this->arFiltros .= $post['prcdatafimvigencia'] ? " and p.prcdatafimvigencia = '".formata_data_sql( $post['prcdatafimvigencia'] )."'" : '';
		$this->arFiltros .= $post['dreid'] ? " and p.dreid = ".$post['dreid'] : '';
		$this->arFiltros .= $post['intid'] ? " and p.intid = ".$post['intid'] : '';
		$this->arFiltros .= $post['prcvalor'] ? " and p.prcvalor = ".$this->retiraPontos( $post['prcvalor'] ) : '';
	}
	
	public function listaProcessos( $filtros ){
		$acao = "";
		
		$sql = "SELECT 
				  	'<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarProcesso('||p.prcid||');\">
						 <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirProcesso('||p.prcid||');\"></center>' as acao,
				  	tp.tpcdsc,
				  	replace(to_char(p.prcnumprocesso::BIGINT, '00000:000000/0000-00'), ':', '.') as prcnumprocesso,
				  	dr.dredsc,
				  	i.intdsc,
				  	s.sitdsc
				FROM 
					seed.processo p
				  	inner join seed.tipoprocesso tp
				    	on tp.tpcid = p.tpcid
				    inner join seed.departamentoresponsavel dr
				    	on dr.dreid = p.dreid
				    left join seed.interessado i
				    	on i.intid = p.intid
				        and i.intstatus = 'A'
				    left join seed.situacao s
				    	on s.sitid = p.sitid
				        and s.sitstatus = 'A'
				WHERE
					p.prcstatus = 'A'
				    and tp.tpcstatus = 'A'
				    and dr.drestatus = 'A'
				    ".$this->arFiltros;

		unset($this->arFiltros);
		
		$cabecalho = array("Ação", "Tipo Processo", "Número Processo", "Depto Responsável", "Interessado", "Situação");
		parent::monta_lista($sql, $cabecalho, 20, 4, 'N','Center','','form');
	}
	
	public function carregaProcesso( $prcid ){
		$sql = "SELECT prcid, cesid, secid, tpcid, dreid, sitid, intid, tilid, gerid,
  					prcnumprocesso, prcnumdocumento, prcdataentradaccon, prcobjeto, prcnumportaria, 
  					prcretificacao, prcvalor, prcdatainiciovigencia, prcdatafimvigencia, prcnumconveniosiafi,
  					prccedente, prcdou, prcstatus
  				FROM seed.processo
  				WHERE prcid = $prcid";
		
		return parent::pegaLinha( $sql );
	}
	
	private function retiraPontos($valor){
		$valor = str_replace(".","", $valor);
		$valor = str_replace(",",".", $valor);
		
		return $valor;
	}
	
	public function listaAnexos( $prcid ){
		$sql = "SELECT
					'<center><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirAnexo('||anx.arqid||');\"></center>' as acao,
					to_char(anx.anxdataanexacao, 'DD/MM/YYYY') as data,
				    tax.taxdsc,
				    anx.anxdsc,
				    arq.arqnome ||'.'||arq.arqextensao as arqnome,
				    arq.arqtamanho||' kbs' as tamanho,
				    usu.usunome
				FROM
					seed.anexos anx
				    inner join seed.tipoanexo tax
				    	on tax.taxid = anx.taxid
				        and tax.taxstatus = 'A' 
				    inner join public.arquivo arq
				    	on arq.arqid = anx.arqid
				        and arq.arqstatus = 'A'
				    inner join seguranca.usuario usu
				    	on usu.usucpf = arq.usucpf
				WHERE
					anx.anxstatus = 'A'
					and prcid = $prcid";
		
		$cabecalho = array("Ação", "Data Inclusão", "Tipo Anexo", "Descrição Arquivo", "Nome Arquivo", "Tamanho", "Responsável");
		parent::monta_lista($sql, $cabecalho, 20, 4, 'N','Center','','form');
	}
	
	public function salvarAnexos( $post ){
		if( $_FILES["arquivo"] && !$_POST['arqid']){
		
			if( $_FILES["arquivo"]["name"] ){
				$campos	= array("prcid"  => $post['prcid'],
								"taxid" => $post['taxid'],
								"anxdataanexacao" => "now()",
								"anxdsc" => "'{$post['anxdsc']}'"
							);
			}
			
			$file = new FilesSimec("anexos", $campos ,"seed");
			$arquivoSalvo = $file->setUpload();
			if($arquivoSalvo){
				echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
				echo"<script>window.location.href = 'seed.php?modulo=principal/anexarDocumento&acao=A';</script>";
				die;	
			}
		} /*elseif( $post['arqid'] ) {
						
		    $sql = "UPDATE seed.anexos SET anxdsc = '{$anxdsc}', anxtermoref = {$anxtermoref} where arqid=".$arqid;
		    parent::executar($sql);
		    parent::commit();
		    echo '<script type="text/javascript"> 
		    		alert("Operação realizada com sucesso!");
		    		window.location.href = "seed.php?modulo=principal/anexarDocumento&acao=A";
		    	  </script>';
		    die;
		}*/
	}

	public function excluirAnexos( $arqid ){
		if( $arqid ){
		    $sql = "DELETE FROM seed.anexos where arqid=".$arqid;
		    parent::executar($sql);
		    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$arqid;
		    parent::executar($sql);
		    parent::commit();
		    
		    $file = new FilesSimec();
			$file->excluiArquivoFisico($arqid);
			
		    echo '<script type="text/javascript"> 
		    		alert("Operação realizada com sucesso!");
		    		window.location.href="seed.php?modulo=principal/anexarDocumento&acao=A";
		    	  </script>';
		    die;
		}
	}
	
	public function carregaHistoricoProcesso( $prcid ){
		$sql = "SELECT
					sit.sitdsc,
					his.hstdsc,
				    to_char(his.hstdata, 'DD/MM/YYYY HH24:MI:SS') as data,
				    usu.usunome
				FROM
					seed.historico his
				    inner join seed.situacao sit
				    	on sit.sitid = his.sitid
				        and sit.sitstatus = 'A'
				    inner join seguranca.usuario usu
				    	on usu.usucpf = his.usucpf
				WHERE
					his.prcid = $prcid
					and his.hststatus = 'A'";

		return parent::carregar( $sql );
	}
	
	public function cadastraSituacaoProcesso( $post ){
		$sql = "INSERT INTO seed.historico(
			prcid, sitid, usucpf,
			hstdsc, hstdata, hststatus
		)
		VALUES(
			".$post['prcid'].",
			".$post['sitid'].",
			'{$_SESSION['usucpf']}',
			'".$post['hstdsc']."',
			now(),
			'A'
		)";

		parent::executar($sql);
	
		$sql = "UPDATE seed.processo SET sitid	= ".$post['sitid']." WHERE prcid = ".$post['prcid'];
		parent::executar($sql);
		
		if( parent::commit() ){
			parent::sucesso( 'principal/alterarSituacao' );
		} else {
			parent::insucesso( 'Falha na operação' );
		}
	}
}
?>