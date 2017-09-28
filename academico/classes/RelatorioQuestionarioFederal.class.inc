<?php
class RelatorioQuestionarioFederal{
	private $g;
	private $q;
	private $p;
	private $s;
	private $it;
	public $arvore;
	private $tipoArvore;
	private $img;
	private $questionario;
    private $perid;
    private $qrpid;
                
    
     
    
    /*
	 * Monta a Tela de impressão do questionário
	 *
	 * Includes necessários: 	include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
	  							include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
	  							include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
	  							include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
	  							include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
	  							include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
	  							include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";
	  							include_once APPRAIZ . "includes/classes/questionario/QImpressao.class.inc";
	  							include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	  
	 * Exemplo de como montá-lo: 	$obImprime = new QImpressao();
	 * 								echo $obImprime->montaArvore();
	 * 
	 * Parametros:
	 * 
	 * cabecalho - Permite habilitar ou não o cabecalho padrão do questionário. Recebe valores 'S'(habilitar) e 'N'(desabilitar). Ex.: $obImprime = new QImpressao( array('cabecalho' => 'N') );
	 * tema 	 - recebe valores de 1 até 4. 1 = Marrom, 2 = verde, 3 = azul, 4 = rosa e cinza por padrão. Ex.: $obImprime = new QImpressao( array('tema' => 1) );
	 * 
	 */
    
	public function __construct(Array $param = null){
		$this->s  = new Sistema();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo();
		$this->p  = new QPergunta();
		$this->it = new QItempergunta();
		$this->r  = new QResposta();
 
		$this->cabecalho = $param['cabecalho'];
		
		$this->perid = $param['perid'];
		$this->qrpid = isset($param['qrpid']) ? $param['qrpid'] : $_REQUEST['qrpid'];
		$this->queid = array( "queid" => isset($param['queid']) ? $param['queid'] : $_REQUEST['queid']);
		
		if( $param['tema'] == 1 ){ //marrom
			$this->borda 	= '#c3baa7';
			$this->titulo 	= '#e9e0cd';
			$this->fundo 	= '#fffeed';
		} elseif( $param['tema'] == 2 ){ //verde
			$this->borda 	= '#aac6a0';
			$this->titulo 	= '#c9dcc3';
			$this->fundo 	= '#e0eadc';
		} elseif( $param['tema'] == 3 ){ //azul
			$this->borda 	= '#96c1be';
			$this->titulo 	= '#bdd8d7';
			$this->fundo 	= '#d8e8e7';
		} elseif( $param['tema'] == 4 ){ //rosa
			$this->borda 	= '#c5b6ca';
			$this->titulo 	= '#dbd1de';
			$this->fundo 	= '#e9e4eb';
		} else { //padrão
			$this->borda 	= '#ffffff';
			$this->titulo 	= '#ffffff';
			$this->fundo 	= '#ffffff';
		}
		
		self::detalheTipoArvore('resposta');
	}
                
	function detalheTipoArvore( $tipoArvore ){
		$this->tipoArvore = $tipoArvore;
                               
		// Sistema
		$this->img['s']['open']                 = '../includes/dtree/img/globe.gif'; 
		$this->img['s']['close']   				= '../includes/dtree/img/globe.gif';
		$this->img['s']['incluirQ']             = ''; 
		$this->img['s']['alterar']              = '';
		$this->img['s']['excluir']              = '';
		// Questionario
		$this->img['q']['open']                 = '../includes/dtree/img/base.gif'; 
		$this->img['q']['close']  				= '../includes/dtree/img/base.gif';
		$this->img['q']['incluirP']             = '';
		$this->img['q']['incluirG']             = '';
		$this->img['q']['alterar']              = '';
		$this->img['q']['excluir']              = '';
		$this->img['q']['imprime']              = '../imagens/print.png';
		// Grupo
		$this->img['g']['open']                 = '../includes/dtree/img/folderopen.gif';
		$this->img['g']['close']  				= '../includes/dtree/img/folder.gif';
		$this->img['g']['incluirP']             = '';
		$this->img['g']['incluirG']             = '';
		$this->img['g']['alterar']              = '';
		$this->img['g']['excluir']              = '';
		// Pergunta
		$this->img['p']['open']                 = '../includes/dtree/img/page.gif';
		$this->img['p']['close']  				= '../includes/dtree/img/page.gif';
		$this->img['p']['respondido']     		= '<img src=\'/imagens/check_p.gif\' title=\'Pergunta respondida\'>';
		$this->img['p']['incluir']              = '';
		$this->img['p']['alterar']              = '';
		$this->img['p']['excluir']              = '';
		// Item
		$this->img['i']['open']                 = '../imagens/ico_config.gif';
		$this->img['i']['close']    			= '../imagens/ico_config.gif';
		$this->img['i']['incluirP']             = '';
		$this->img['i']['incluirG']             = '';
		$this->img['i']['alterar'] 				= '';
		$this->img['i']['excluir'] 				= '';
	}
	
	function limitaString( $titulo ){
		if(strlen($titulo) > 60){
			$titulo = substr($titulo,0,60) . '...';
		}
		return $titulo;
	}
	
	function quebraTitulo( $titulo ){
		$titulo = wordwrap($titulo, 60, "<br />");
		return $titulo;
	}
                
	function montaArvore( $sisid = null ){
		echo "<style>

		table.tabelaQuestionario {
			/* background-color:#cccccc; */
			font-family: arial, sans-serif;
			font-size: 14px;
		}

		table.tabelaGrupo {
			background-color:#d1d1d1;
			font-family: arial, sans-serif;
			font-size: 14px;
		}
		
		td.bordaDireita {
			background-color:#ffffff;
			font-family: arial, sans-serif;
			font-size: 14px;
			width:300px;
		}

		.fieldGrupoTitulo {
			color:#000000;
			font-family:arial, sans-serif;
			font-size: 11px;
			font-weight: bold;
			
		}
		
		.fieldGrupo {
			background-color:#ffffff;
			font-family:arial, sans-serif;
			font-size: 12px;
			padding-bottom:10px;
			margin:10px;
			border:1px solid ".$this->borda.";
			
		}
		
		.divQuestionario {
			text-align:left;
		}
		
		.divTituloQuestionario {
			text-align:center;
			font-family:arial, sans-serif;
			font-size: 16px;
			font-weight: bold;
		}
		
		.divPergunta {
			font-family:verdana, arial, sans-serif;
			font-size: 11px;
			background-color:".$this->fundo.";
			margin-left:10px;
			margin-right:10px;
			margin-top:5px;
			padding-bottom:5px;
			border:1px solid ".$this->borda.";
		}

		.divTituloPergunta {
			background-color:".$this->titulo.";
			padding-top:3px;
			padding-bottom:5px;
			border-bottom:1px solid ".$this->borda.";
		}
		
		.divResposta {
			font-family:verdana, arial, sans-serif;
			font-size: 11px;
			background-color:".$this->fundo.";
			padding-top:3px;
			padding-left:10px;
		}
		
		</style>";
		if(!$this->cabecalho){
			echo '<table width="100%" border="0" cellpadding="0" cellspacing="0" style="font-family:arial, sans-serif;">
				<tr>
					<td valign="top" width="50" rowspan="2">
						<img src="../imagens/brasao.gif" width="45" height="45" border="0">
					</td>
					<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
						<font style="font-size:11px;">
							'. NOME_SISTEMA. '<br/>
							'.$_SESSION['sisdsc'].'
						</font>
					</td>
					<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
						<font style="font-size:11px;">
							Impresso por: <b>'.$_SESSION['usunome'].'</b><br/>
							Data/Hora da Impressão: <b>'.date( 'd/m/Y - H:i:s' ).'</b><br/>
						</font>
					</td>
				</tr>
					<tr><td>&nbsp;</td></tr>
			</table>';
		}elseif($this->cabecalho != 'N' && $this->cabecalho){
			echo $this->cabecalho;
		}
		
		self::montaArvoreQ( $this->queid );    

	}
                
	function montaArvoreQ($queid){
		$obPai = $this->q->pegaUmObjeto($queid);
		//Filtra Questionarios
		$arrObQuestionario = (array) $this->q->listaObjeto( array($obPai), array('queid', 'quetitulo') );
		foreach($arrObQuestionario as $obQuestionario){
			echo "<div class='divQuestionario'><div class='divTituloQuestionario'><b>".$obQuestionario->quetitulo."</b></div></center>";						
			//Busca Perguntas associadas
			self::montaArvoreP($obQuestionario);
			//Busca Grupo Pergunta associadas
			self::montaArvoreG($obQuestionario);
			echo "</div>";
		}
	}
	
	function montaArvoreG($obPai){
		//Filtra Grupos vinculadas
		$arrObGrupoPergunta = (array) $this->g->listaObjeto(array( $obPai ), array("grpid, grptitulo, grpordem"));
		foreach ($arrObGrupoPergunta as $obGrupoPergunta){
			echo "<div class='divPergunta'>";
			echo "<div class='divTituloPergunta' style='background-color:#999999' ><b>".$obGrupoPergunta->grptitulo."</b></div>";
			//Busca Perguntas associadas
			self::montaArvoreP($obGrupoPergunta);
			//Busca Grupo Pergunta associadas
			if(!(get_class($obPai) == 'ItemPergunta')){//Se for filho de um item então não inclui mais grupos!
				self::montaArvoreG($obGrupoPergunta);
			}
			echo "</div>";
		}	
	}

	function montaArvoreP( $obPai ){
		// Filtra perguntas vinculadas
		$arrObPergunta = (array) $this->p->listaObjeto(array( $obPai ), array("perid, pertitulo, perordem, pertipo"));
		
		foreach ($arrObPergunta as $obPergunta){
			if($obPergunta->pertipo != 'TA'){
				
				//echo "<div class='divPergunta'>";
				echo "<div class='divTituloPergunta'>".$obPergunta->pertitulo;
				
				global $db;
				
				$ano = $_REQUEST['ano'];
				$entidade = $_REQUEST['entidade'];
				$tipoentidade = $_REQUEST['tipoentidade'];
				
				if( $entidade == 'todas' ){
	
					//Primeiro semestre
					$sql = "SELECT 			DISTINCT it.qrpid 
							FROM			academico.assistenciaestudantil aet
	            			INNER JOIN 		academico.itemassistencia it ON it.aetdid = aet.aetdid
							WHERE			aetsemestre = 1 
							AND				aetano = {$ano}";
					
					$arrQrpid1 = $db->carregarColuna($sql);
					$strQrpid1 = implode(", ", $arrQrpid1);
					
					//Segundo semestre
					$sql = "SELECT 			DISTINCT it.qrpid 
							FROM   			academico.assistenciaestudantil aet
	            			INNER JOIN 		academico.itemassistencia it on it.aetdid = aet.aetdid
							WHERE			aetsemestre = 2 
							AND				aetano = {$ano}";
					$arrQrpid2 = $db->carregarColuna($sql);
					$strQrpid2 = implode(", ", $arrQrpid2);
					
				} else {
					if ($tipoentidade == 'universidade'){
						$entidades = str_replace("\'", "'",trim($entidade,","));
						if(!empty($entidades)){
							$sql = "SELECT cmpentid FROM academico.unicampus WHERE unientid IN ({$entidades})";
							$entidade = implode(",", $db->carregarColuna($sql));
						} else {
							$entidade = 'null';
						}
					}
					// Primeiro semestre
					$sql = "SELECT		  	DISTINCT it.qrpid 
							FROM			academico.assistenciaestudantil aet
	            			INNER JOIN 		academico.itemassistencia it ON it.aetdid = aet.aetdid
							WHERE			aetsemestre = 1 
							AND				entid IN ({$entidade}) 
							AND				aetano = {$ano}";
					$arrQrpid1 = $db->carregarColuna($sql);
					$strQrpid1 = implode(", ", $arrQrpid1);
					
					// Segundo semestre
					$sql = "SELECT 			DISTINCT it.qrpid 
							FROM   			academico.assistenciaestudantil aet
	            			INNER JOIN 		academico.itemassistencia it ON it.aetdid = aet.aetdid
							WHERE			aetsemestre = 2 
							AND				entid IN ({$entidade}) 
							AND				aetano = {$ano}";
					$arrQrpid2 = $db->carregarColuna($sql);
					$strQrpid2 = implode(", ", $arrQrpid2);
				}
				
				if($strQrpid1 == '' || $strQrpid2 == '' ){
					echo " Nenhum registro encontrado</div>";
					//echo "</div>";
					return;
				}
				
				if ($strQrpid1 != '' || $strQrpid2 != '' ){
					$arrR1 = array( "perid" => $obPergunta->perid, "generica" => 'qrpid in ('.$strQrpid1.') AND resdsc <> \'\' AND resdsc <> \'0\'' );
					$arrR2 = array( "perid" => $obPergunta->perid, "generica" => 'qrpid in ('.$strQrpid2.') AND resdsc <> \'\' AND resdsc <> \'0\'' );
					
					//pergutas que devem ter a reposta por média, nao soma.
					$arrPerguntas =array (3066,3084,3091,3093,3094,3096,3098,3099);
					//perguntas que devem ter o consalidado como soma s1+s2
					$arrPerguntas2 =array (3101,3102,3105,3106);
					//pergutas que devem ter o consalidado com a seguinte formula (m1+m2)/2
					$arrPerguntas3 =array (3085,3086,3089,3090,3091,3092,3093,3094,3097,3098,3099,3100);
	
					if( $obPergunta->pertipo != 'TA' ){
						if (in_array($obPergunta->perid, $arrPerguntas))
						{
							$resposta = $this->r->pegaUmObjeto( $arrR1, 'avg(REPLACE(resdsc, \',\', \'.\')::numeric) as resdsc' );
							$resposta2 = $this->r->pegaUmObjeto( $arrR2, 'avg(REPLACE(resdsc, \',\', \'.\')::numeric) as resdsc' );
						}
						else
						{
							$resposta = $this->r->pegaUmObjeto( $arrR1, 'sum(REPLACE(resdsc, \',\', \'.\')::numeric) as resdsc' );
							$resposta2 = $this->r->pegaUmObjeto( $arrR2, 'sum(REPLACE(resdsc, \',\', \'.\')::numeric) as resdsc' );
						}
					} else {
						$resposta = $this->r->pegaUmObjeto( $arrR1, 'sum(REPLACE(resdsc, \',\', \'.\')::numeric) as resdsc' );
					}
					
					$arrItemResp = $this->p->perguntaRespondidaComItem( $obPergunta->perid, $this->qrpid );

					if( $resposta->resdsc || $resposta2->resdsc ){
						if( $obPergunta->pertipo != 'EXT' && $obPergunta->pertipo != 'EXF') {
							$sem1 = $resposta->resdsc ?  $resposta->resdsc: '&nbsp;';
							$sem2 = $resposta2->resdsc ? $resposta2->resdsc: '&nbsp;';
							$soma = ($resposta->resdsc) + ($resposta2->resdsc);
							//$sem2 = $resposta2->resdsc ? number_format($resposta2->resdsc, 2, ",", "") : '&nbsp;';
							$soma = number_format($soma, 2, ",", "");
							$consolid = 0;
							
							$consolid = $sem2;
							
							if(!in_array($obPergunta->perid, $arrPerguntas2)){
								$consolid = ($sem1+$sem2);
							}
							
							if(!in_array($obPergunta->perid, $arrPerguntas3)){
								$consolid = ($sem1+$sem2)/2;
							}
							
							$consolid = number_format($consolid,0,",",".");
							$sem1 = $resposta->resdsc ?  number_format($sem1,0,",",".") : '&nbsp;';
							$sem2 = $resposta2->resdsc ? number_format($sem2,0,",",".") : '&nbsp;';
								
							echo "<div class='divResposta'>";
							echo "<table border=1 style='border-collapse:collapse'>
										<tr>
											<td><B>&nbsp;1º Sem ".$_REQUEST['ano']."&nbsp;</td>
											<td><B>&nbsp;2º Sem ".$_REQUEST['ano']."&nbsp;</td>
											<td><B>&nbsp;Consolidado&nbsp;</td>
										</tr>";
							echo "<tr>
										<td>&nbsp;".$sem1."&nbsp;</td>
										<td>&nbsp;".$sem2."&nbsp;</td>
										<td>&nbsp;".$consolid."&nbsp;</td>
									</tr>";
							echo "</table></div>";
						}
					}
				
					if($obPergunta->pertipo == 'EXF'){
						echo "<div class='divResposta'>";
						$obMonta = new CampoExternoControle();
						if( method_exists ( $obMonta , retornoRelatorio )){
							$obMonta->retornoRelatorio( $this->qrpid, $obPergunta->perid );
						} else {
							echo "Campo Externo";
						}
						echo "</div>";
					}
					
					if($obPergunta->pertipo == 'EXT'){
						echo "<div class='divResposta'>";
						$campo = new MontatabelaFederal();		
						$campo->montaNovaTabela( $obPergunta->perid, $this->qrpid, 70, 1);
						$campo->show();
						echo "</div>";
					}
					
					if(!$arrItemResp && !$resposta->resdsc && $obPergunta->pertipo != 'EXT' && $obPergunta->pertipo != 'EXF'){
						echo "<div class='divResposta'>";
						echo "Não informado.";
						echo "</div>";
					}
				}
					
				if($arrItemResp){
					echo $obpergunta->pertitulo;
					if( $obPergunta->pertipo == 'CK' ){
						$respostaCK = $this->r->listaObjeto( array( "qrpid" => $this->qrpid, "perid" => $obPergunta->perid ) );
						echo "<div class='divResposta'>";
						$pegaItens = $this->it->carregaItensPergunta( $obPergunta->perid, array("itpid", "itptitulo") );
						foreach($pegaItens as $item){
							$tem = false;
							foreach($respostaCK as $resp){
								if( $item['itpid'] == $resp->itpid ){
									$arrItemResp = $this->it->listaObjeto( array( "itpid" => $resp->itpid, "qrpid" => $this->qrpid), array("itpid, itptitulo") );
									$arrObPergunta = (array) $this->p->listaObjeto(array( "qrpid" => $this->qrpid, "itpid" => $arrItemResp[0]->itpid ), array("perid, pertitulo, perordem, pertipo"));
									echo "<img src=\"/imagens/checked.gif\" align='bottom' >&nbsp;";
									echo $arrItemResp[0]->itptitulo;
									if( $arrObPergunta[0] ){
										self::montaArvoreP( $arrObPergunta[0] );
									}
									echo "<br />";
									$tem = true;
								}
							}
							if( $tem == false ){
								echo "<img src=\"/imagens/check.gif\" align='bottom' >&nbsp;";
								echo $item['itptitulo'];
								echo "<br />";
							}
						}
						echo "</div>";
					} else {
						self::montaArvoreI($obPergunta, $arrItemResp);
					}
				}
				if ($strQrpid1 != '' || $strQrpid2 != '' ){
					echo "</div>";
				}
			}
		}
	}
	
	function montaArvoreI( $obPai, $arrItemResp = null ){
		$where[0] = $obPai;
		if($arrItemResp[0]){
			$where['itpid'] = $arrItemResp;
		}
		// Filtra perguntas vinculadas
		$arrObItPergunta = (array) $this->it->listaObjeto($where, array("itpid, itptitulo"));
		foreach ($arrObItPergunta as $obItPergunta){
			echo "<div class='divResposta'>";
			echo $obItPergunta->itptitulo."<br>";
			
			// Busca Perguntas associadas
			$verifica = $this->it->verificaGrupo( $obItPergunta->itpid );
			if(!($verifica)){
				self::montaArvoreP($obItPergunta);
			}

			// Busca Grupo Pergunta associadas
			unset( $verifica );
			$verifica = $this->it->verificaPergunta( $obItPergunta->itpid );
			if(!($verifica)){
				self::montaArvoreG($obItPergunta);
			}
			echo "</div>";
		}
	}
}