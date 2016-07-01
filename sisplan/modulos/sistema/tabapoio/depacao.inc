<?php

$modulo = $_REQUEST['modulo'] ;

$usucpf = formatar_cpf($_SESSION['usucpf']);

$tipo = array(38100 => array('codigo'=>38100, 'descricao'=>'Programa Estratégico'),
			  38200 => array('codigo'=>38200, 'descricao'=>'Classificação Cultural'),
			  38300 => array('codigo'=>38300, 'descricao'=>'Emendas'),
			  38400 => array('codigo'=>38400, 'descricao'=>'Regionalização'),
			  38500 => array('codigo'=>38500, 'descricao'=>'Instrumento'),
			  38600 => array('codigo'=>38600, 'descricao'=>'Proponente'),
			  38700 => array('codigo'=>38700, 'descricao'=>'Produto Secundário'),
			  99000 => array('codigo'=>99000, 'descricao'=>'PPA')
			  );
			  
			
$tabelas = array( 38100 => array(38110=>array('codigo'=>38110, 'descricao'=>'Programa'),
								 38120=>array('codigo'=>38120, 'descricao'=>'Ação')
								),
				  38200 => array(38210=>array('codigo'=>38210, 'descricao'=>'Unidade Executora'),
				  				 38220=>array('codigo'=>38220, 'descricao'=>'Área'),
				  				 38230=>array('codigo'=>38230, 'descricao'=>'Segmento'),
				  				 38240=>array('codigo'=>38240, 'descricao'=>'Tipo de Setor Cultural'),
				  				 38250=>array('codigo'=>38250, 'descricao'=>'Tipo Anexo PI')
				  				),
				  38300 => array(38310=>array('codigo'=>38310, 'descricao'=>'Tipo de Autor'),
				  				 38320=>array('codigo'=>38320, 'descricao'=>'Partido Político'),
				  				 38330=>array('codigo'=>38330, 'descricao'=>'Autor')
				  				),
				  38400 => array(38410=>array('codigo'=>38410, 'descricao'=>'País'),
				  				 38420=>array('codigo'=>38420, 'descricao'=>'Região Administrativa - DF')
				  				),
				  38500 => array(38510=>array('codigo'=>38510, 'descricao'=>'Tipo de Instrumento')),
				  38600 => array(38610=>array('codigo'=>38610, 'descricao'=>'Tipo de Proponente')),
				  38700 => array(38710=>array('codigo'=>38710, 'descricao'=>'Produto'),
				  				 38720=>array('codigo'=>38720, 'descricao'=>'Unidade de Medida')
				  				),
				  99000 => array(99010=>array('codigo'=>99010, 'descricao'=>'Ação'),
				  				 99020=>array('codigo'=>99020, 'descricao'=>'Programa')
				  				)
				  );


$buscaTabela = array();			  
if ( $_REQUEST['idTipo'] )
	$buscaTabela = $tabelas[$_REQUEST['idTipo']];

	
	

switch ($_REQUEST['evento'])
{
	case 'exibirXML':
	
		include APPRAIZ."includes/Snoopy.class.php";
		
		$s = new Snoopy;
		
		$s->agent 		= "";
		$s->_isproxy 	= false;
		$s->proxy_host	= "";
		$s->proxy_port	= "";
		$s->proxy_user	= "";
		$s->proxy_pass	= "";
		$s->results		= "";
		
		$header = sprintf(
			"%sservico.php?usucpf=%s&ususenha=%s&idTabela=%s&modulo=%s&acao=%s",
			URL_MINC,
			$_REQUEST['usucpf'],
			$_REQUEST['ususenha'],
			$_REQUEST['tabela'],
			$_REQUEST['moduloSIMC'],
			$_REQUEST['acaoSIMC']
		);
//dbg($header);
		$s->fetch($header);
		
		$aux = substr($s->results, strpos($s->results,'<?xml') );
		
		//dbg($s->fetch($header));
		switch ( $_REQUEST['idTabela'] )
		{
			case 38110://Programa
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfProgramaEstrategico>') )."</ArrayOfProgramaEstrategico>";
				//$_xml = str_replace('\"','"',$s->results);
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->ProgramaEstrategico as $programa)
					$arId .= $programa->preid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.programaestrategico SET prestatus = 'I' WHERE preid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->ProgramaEstrategico as $programa)
				{
					$sql = "SELECT * FROM planointerno.programaestrategico WHERE preid= ".$programa->preid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.programaestrategico (preid, predsc, prestatus) 
								VALUES 
									(".utf8_decode($programa->preid).", '".utf8_decode($programa->predsc)."', '".utf8_decode($programa->prestatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.programaestrategico
								SET
									predsc = '".utf8_decode($programa->predsc)."',
									prestatus = '".utf8_decode($programa->prestatus)."'
								WHERE 
									preid = ".utf8_decode($programa->preid);
					}
					$db->executar($sql);
				}
				break;	
				
			case 38120://Ação
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfAcaoEstrategica>') )."</ArrayOfAcaoEstrategica>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->AcaoEstrategica as $acao)
					$arId .= $acao->aesid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.acaoestrategica SET aesstatus = 'I' WHERE aesid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->AcaoEstrategica as $acao)
				{
					$sql = "SELECT * FROM planointerno.acaoestrategica WHERE aesid= ".$acao->aesid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.acaoestrategica (aesid, preid, aesdsc, aesstatus) 
								VALUES 
									(".utf8_decode($acao->aesid).", ".utf8_decode($acao->preid).", '".utf8_decode($acao->aesdsc)."', '".utf8_decode($acao->aesstatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.acaoestrategica
								SET
									preid = ".utf8_decode($acao->preid).",
									aesdsc = '".utf8_decode($acao->aesdsc)."',
									aesstatus = '".utf8_decode($acao->aesstatus)."'
								WHERE 
									aesid = ".utf8_decode($acao->aesid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38210://Unidade Executora
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfUnidadeExecutora>') )."</ArrayOfUnidadeExecutora>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->UnidadeExecutora as $uniExec)
					$arId .= $uniExec->uexid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.unidadeexecutora SET uexstatus = 'I' WHERE uexid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->UnidadeExecutora as $uniExec)
				{
					$sql = "SELECT * FROM planointerno.unidadeexecutora WHERE uexid= ".$uniExec->uexid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.unidadeexecutora (uexcod, uexsigla, uexdsc, uexstatus, uexid) 
								VALUES 
									('".utf8_decode($uniExec->uexcod)."', '".utf8_decode($uniExec->uexsigla)."', '".utf8_decode($uniExec->uexdsc)."', '".utf8_decode($uniExec->uexstatus)."', ".utf8_decode($uniExec->uexid).")";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.unidadeexecutora
								SET
									uexcod = '".utf8_decode($uniExec->uexcod)."',
									uexsigla = '".utf8_decode($uniExec->uexsigla)."',
									uexdsc = '".utf8_decode($uniExec->uexdsc)."',
									uexstatus = '".utf8_decode($uniExec->uexstatus)."'
								WHERE 
									uexid = ".utf8_decode($uniExec->uexid);
					}
					$db->executar($sql);
				}
				break;
					
			case 38220://Área
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfArea>') )."</ArrayOfArea>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->Area as $area)
					$arId .= $area->areid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.area SET arestatus = 'I' WHERE areid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->Area as $area)
				{
					$sql = "SELECT * FROM planointerno.area WHERE areid= ".$area->areid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.area (areid, aredsc, arestatus) 
								VALUES 
									(".utf8_decode($area->areid).", '".utf8_decode($area->aredsc)."', '".utf8_decode($area->arestatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.area
								SET
									aredsc = '".utf8_decode($area->aredsc)."',
									arestatus = '".utf8_decode($area->arestatus)."'
								WHERE 
									areid = ".utf8_decode($area->areid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38230://Segmento 
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfSegmento>') )."</ArrayOfSegmento>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->Segmento as $seg)
					$arId .= $seg->segid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.segmento SET segstatus = 'I' WHERE segid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->Segmento as $seg)
				{
					$sql = "SELECT * FROM planointerno.segmento WHERE segid= ".$seg->segid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.segmento (segid, segdsc, segstatus) 
								VALUES 
									(".utf8_decode($seg->segid).", '".utf8_decode($seg->segdsc)."', '".utf8_decode($seg->segstatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.segmento
								SET
									segdsc = '".utf8_decode($seg->segdsc)."',
									segstatus = '".utf8_decode($seg->segstatus)."'
								WHERE 
									segid = ".utf8_decode($seg->segid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38240://Tipo de Setor Cultural
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfTipoSetorCultural>') )."</ArrayOfTipoSetorCultural>";
				$xml = simplexml_load_string($aux);
				 
				$arId = '';
				foreach($xml->TipoSetorCultural as $cul)
					$arId .= $cul->tscid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.tiposetorcultural SET tscstatus = 'I' WHERE tscid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->TipoSetorCultural as $cul)
				{
					$sql = "SELECT * FROM planointerno.tiposetorcultural WHERE tscid= ".$cul->tscid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.tiposetorcultural (tscid, tscdsc, tscstatus) 
								VALUES 
									(".utf8_decode($cul->tscid).", '".utf8_decode($cul->tscdsc)."', '".utf8_decode($cul->tscstatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.tiposetorcultural
								SET
									tscdsc = '".utf8_decode($cul->tscdsc)."',
									tscstatus = '".utf8_decode($cul->tscstatus)."'
								WHERE 
									tscid = ".utf8_decode($cul->tscid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38250://Tipo Anexo PI
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfTipoAnexoPI>') )."</ArrayOfTipoAnexoPI>";
				$xml = simplexml_load_string($aux);
				/*
				$arId = '';
				foreach($xml->TipoAnexoPI as $ane)
					$arId .= $ane->tapid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE pde.tipoanexopi SET taplegal = 'FALSE' WHERE tapid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				*/
				foreach($xml->TipoAnexoPI as $ane)
				{
					$sql = "SELECT * FROM pde.tipoanexopi WHERE tapid= ".$ane->tapid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									pde.tipoanexopi (tapid, tapdescricao, taplegal) 
								VALUES 
									(".utf8_decode($ane->tapid).", '".utf8_decode($ane->tapdescricao)."', '".utf8_decode($ane->taplegal)."')";
					}
					else 
					{
						$sql = "UPDATE 
									pde.tipoanexopi
								SET
									tapdescricao = '".utf8_decode($ane->tapdescricao)."',
									taplegal = '".utf8_decode($ane->taplegal)."'
								WHERE 
									tapid = ".utf8_decode($ane->tapid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38310://Tipo de Autor
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfTipoAutor>') )."</ArrayOfTipoAutor>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->TipoAutor as $aut)
					$arId .= $aut->tauid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.tipoautor SET taustatus = 'I' WHERE tauid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->TipoAutor as $aut)
				{
					$sql = "SELECT * FROM planointerno.tipoautor WHERE tauid= ".$aut->tauid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.tipoautor (tauid, taudsc, taustatus) 
								VALUES 
									(".utf8_decode($aut->tauid).", '".utf8_decode($aut->taudsc)."', '".utf8_decode($aut->taustatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.tipoautor
								SET
									taudsc = '".utf8_decode($aut->taudsc)."',
									taustatus = '".utf8_decode($aut->taustatus)."'
								WHERE 
									tauid = ".utf8_decode($aut->tauid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38320://Partido Político
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfPartidoPolitico>') )."</ArrayOfPartidoPolitico>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->PartidoPolitico as $part)
					$arId .= $part->ppoid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.partidopolitico SET ppostatus = 'I' WHERE ppoid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->PartidoPolitico as $part)
				{
					$sql = "SELECT * FROM planointerno.partidopolitico WHERE ppoid= ".$part->ppoid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.partidopolitico (ppoid, pposigla, pponumero, ppodsc, ppostatus) 
								VALUES 
									(".utf8_decode($part->ppoid).", '".utf8_decode($part->pposigla)."', '".utf8_decode($part->pponumero)."', '".utf8_decode($part->ppodsc)."', '".utf8_decode($part->ppostatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.partidopolitico
								SET
									pposigla = '".utf8_decode($part->pposigla)."',
									pponumero = '".utf8_decode($part->pponumero)."',
									ppodsc = '".utf8_decode($part->taudsc)."',
									ppostatus = '".utf8_decode($part->ppostatus)."'
								WHERE 
									ppoid = ".utf8_decode($part->ppoid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38330://Autor
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfAutorEmenda>') )."</ArrayOfAutorEmenda>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->AutorEmenda as $autE)
					$arId .= $autE->aueid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.autoremenda SET auestatus = 'I' WHERE aueid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->AutorEmenda as $autE)
				{
					$sql = "SELECT * FROM planointerno.autoremenda WHERE aueid= ".$autE->aueid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.autoremenda (aueid, ppoid, tauid, auedsc, auestatus) 
								VALUES 
									(".utf8_decode($autE->aueid).", ".utf8_decode($autE->ppoid).", ".utf8_decode($autE->tauid).", '".utf8_decode($autE->auedsc)."', '".utf8_decode($autE->auestatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.autoremenda
								SET
									ppoid = ".utf8_decode($autE->ppoid).",
									tauid = ".utf8_decode($autE->tauid).",
									auedsc = '".utf8_decode($autE->auedsc)."',
									auestatus = '".utf8_decode($autE->auestatus)."'
								WHERE 
									aueid = ".utf8_decode($autE->aueid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38410://País
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfPais>') )."</ArrayOfPais>";
				$xml = simplexml_load_string($aux);
				/*
				$arId = '';
				foreach($xml->Pais as $pa)
					$arId .= $pa->paiid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE territorios.pais SET auestatus = 'I' WHERE paiid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				*/
				foreach($xml->Pais as $pa)
				{
					$sql = "SELECT * FROM territorios.pais WHERE paiid= ".$pa->paiid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									territorios.pais (paiid, paidescricao) 
								VALUES 
									(".utf8_decode($pa->paiid).", '".utf8_decode($pa->paidescricao)."')";
					}
					else 
					{
						$sql = "UPDATE 
									territorios.pais
								SET
									paidescricao = '".utf8_decode($pa->paidescricao)."'
								WHERE 
									paiid = ".utf8_decode($pa->paiid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38420://Região Administrativa - DF
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfRegiaoAdministrativa>') )."</ArrayOfRegiaoAdministrativa>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->RegiaoAdministrativa as $reg)
					$arId .= $reg->rgaid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.regiaoadministrativa SET rgastatus = 'I' WHERE rgaid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->RegiaoAdministrativa as $reg)
				{
					$sql = "SELECT * FROM planointerno.regiaoadministrativa WHERE rgaid= ".$reg->rgaid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.regiaoadministrativa (rgaid, rgadsc, rgastatus) 
								VALUES 
									(".utf8_decode($reg->rgaid).", '".utf8_decode($reg->rgadsc)."', '".utf8_decode($reg->rgastatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.regiaoadministrativa
								SET
									rgadsc = '".utf8_decode($reg->rgadsc)."',
									rgastatus = '".utf8_decode($reg->rgastatus)."'
								WHERE 
									rgaid = ".utf8_decode($reg->rgaid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38510://Tipo de Instrumento
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfTipoInstrumento>') )."</ArrayOfTipoInstrumento>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->TipoInstrumento as $instr)
					$arId .= $instr->tpiid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.tipoinstrumento SET tpistatus = 'I' WHERE tpiid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->TipoInstrumento as $instr)
				{
					$sql = "SELECT * FROM planointerno.tipoinstrumento WHERE tpiid= ".$instr->tpiid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.tipoinstrumento (tpiid, tpidsc, tpistatus) 
								VALUES 
									(".utf8_decode($instr->tpiid).", '".utf8_decode($instr->tpidsc)."', '".utf8_decode($instr->tpistatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.tipoinstrumento
								SET
									tpidsc = '".utf8_decode($instr->tpidsc)."',
									tpistatus = '".utf8_decode($instr->tpistatus)."'
								WHERE 
									tpiid = ".utf8_decode($instr->tpiid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38610://Tipo de Proponente
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfTipoProponente>') )."</ArrayOfTipoProponente>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->TipoProponente as $prop)
					$arId .= $prop->tppid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.tipoproponente SET tppstatus = 'I' WHERE tppid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->TipoProponente as $prop)
				{
					$sql = "SELECT * FROM planointerno.tipoproponente WHERE tppid= ".$prop->tppid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.tipoproponente (tppid, tppdsc, tppstatus) 
								VALUES 
									(".utf8_decode($prop->tppid).", '".utf8_decode($prop->tppdsc)."', '".utf8_decode($prop->tppstatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.tipoproponente
								SET
									tppdsc = '".utf8_decode($prop->tppdsc)."',
									tppstatus = '".utf8_decode($prop->tppstatus)."'
								WHERE 
									tppid = ".utf8_decode($prop->tppid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38710://Produto
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfProduto>') )."</ArrayOfProduto>";
				$xml = simplexml_load_string($aux);
				
				$arId = '';
				foreach($xml->Produto as $prod)
					$arId .= $prod->pduid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.produto SET pdustatus = 'I' WHERE pduid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->Produto as $prod)
				{
					$sql = "SELECT * FROM planointerno.produto WHERE pduid= ".$prod->pduid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.produto (pduid, pdudsc, pdustatus) 
								VALUES 
									(".utf8_decode($prod->pduid).", '".utf8_decode($prod->pdudsc)."', '".utf8_decode($prod->pdustatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.produto
								SET
									pdudsc = '".utf8_decode($prod->pdudsc)."',
									pdustatus = '".utf8_decode($prod->pdustatus)."'
								WHERE 
									pduid = ".utf8_decode($prod->pduid);
					}
					$db->executar($sql);
				}
				break;
				
			case 38720://Unidade de Medida
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfUnidadeMedida>') )."</ArrayOfUnidadeMedida>";
				$xml = simplexml_load_string($aux);
				 
				$arId = '';
				foreach($xml->UnidadeMedida as $unim)
					$arId .= $unim->udaid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE planointerno.unidademedida SET udastatus = 'I' WHERE udaid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->UnidadeMedida as $unim)
				{
					$sql = "SELECT * FROM planointerno.unidademedida WHERE udaid= ".$unim->udaid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO 
									planointerno.unidademedida (udaid, udadsc, udastatus) 
								VALUES 
									(".utf8_decode($unim->udaid).", '".utf8_decode($unim->udadsc)."', '".utf8_decode($unim->udastatus)."')";
					}
					else 
					{
						$sql = "UPDATE 
									planointerno.unidademedida
								SET
									udadsc = '".utf8_decode($unim->udadsc)."',
									udastatus = '".utf8_decode($unim->udastatus)."'
								WHERE 
									udaid = ".utf8_decode($unim->udaid);
					}
					$db->executar($sql);
				}
				break;

			case 99010://Monitóra - Ação
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfMonitoraAcao>') )."</ArrayOfMonitoraAcao>";
				//dbg($s->results);
				//dbg($aux,1);
				$xml = simplexml_load_string($aux);
				 
				//dbg($xml,1);
				
				$arId = '';
				foreach($xml->MonitoraAcao as $monacao)
					$arId .= $monacao->acaid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE monitora.acao SET acastatus = 'I' WHERE acaid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->MonitoraAcao as $monacao)
				{
					$sql = "SELECT * FROM monitora.acao WHERE acaid= ".$monacao->acaid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO monitora.acao (";
						foreach( $monacao as $x=>$y)
							$sql .= $x.", ";

						$sql .= ") VALUES (";
						
						foreach( $monacao as $x=>$y)
							$sql .= "'".utf8_decode($y)."', ";
							
						$sql .= ")";
						
						$sql = str_replace(", )", ")", $sql);
					}
					else 
					{
						$sql = "UPDATE monitora.acao SET ";
						
						foreach( $monacao as $x=>$y)
							$sql .= $x." = '".utf8_decode($y)."', ";
							
						$sql .= "WHERE acaid = ".utf8_decode($monacao->acaid);
						
						$sql = str_replace(", WHERE", " WHERE", $sql);
					}
					$db->executar($sql);
				}
				break;
				
			case 99020://Monitóra - Programa
				$aux = substr($aux, strpos($aux,'<?xml'), strpos($aux,'</ArrayOfMonitoraPrograma>') )."</ArrayOfMonitoraPrograma>";
				$xml = simplexml_load_string($aux);
				 
				$arId = '';
				foreach($xml->MonitoraPrograma as $monprog)
					$arId .= $monprog->prgid.",";
				
				$arId = $arId.",";
				$sql = "UPDATE monitora.programa SET prgstatus = 'I' WHERE prgid NOT IN (". str_replace(",,","",$arId) .")";
				
				$db->executar($sql);
				
				foreach($xml->MonitoraPrograma as $monprog)
				{
					$sql = "SELECT * FROM monitora.programa WHERE prgid= ".$monprog->prgid;
					$result = $db->recuperar($sql);
					if ( $result == '' )
					{
						$sql = "INSERT INTO monitora.programa (";
						foreach( $monprog as $x=>$y)
							$sql .= $x.", ";

						$sql .= ") VALUES (";
						
						foreach( $monprog as $x=>$y)
							$sql .= "'".utf8_decode($y)."', ";
							
						$sql .= ")";
						
						$sql = str_replace(", )", ")", $sql);
					}
					else 
					{
						$sql = "UPDATE monitora.programa SET ";
						
						foreach( $monprog as $x=>$y)
							$sql .= $x." = '".utf8_decode($y)."', ";
							
						$sql .= "WHERE prgid = ".utf8_decode($monprog->prgid);
						
						$sql = str_replace(", WHERE", " WHERE", $sql);
					}
					$db->executar($sql);
				}
				break;
		}
		
		try 
		{
			$db->commit();
		}
		catch ( Exception $e )
		{
			$db->rollback();
		}
		$db->sucesso( $modulo );
		
		break;
		
	default:
		break;
}

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$parametros = array('','','');
$db->cria_aba($abacod_tela,$url,$parametros);

//título da página
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<script>
function buscarTabelas( tip )
{
	document.formulario.idTipo.value = tip;
	
	document.formulario.submit();
}

function recuperaDados ( tab )
{
	document.formulario.idTipo.value = document.formulario.tipo.value;
	
	document.formulario.idTabela.value = tab;
	
	//document.formulario.submit();
}

function validar()
{
		var d = document;	
		var f = d.formulario;
		var msg = '';
		
		if ( f.tipo.value == '' ) 
			msg += 'Tipo\n';	
		
		if (f.tabela.value == '')
			msg += 'Tabela de Apoio\n';

			
		if ( !validar_cpf( f.usucpf.value ) ) 
			msg += 'O cpf informado não é válido\n';


		if ( f.ususenha.value == "" )
			msg += 'Senha\n';

			
		if (msg != ''){
			alert('Os campos listados são obrigatórios e devem ser preenchidos:\n'+msg);
			return false;
		}
		
		f.evento.value = 'exibirXML';
		
		f.submit();
}


function controlar_foco_cpf( evento ) {
	if ( window.event || evento.which ) {
		if ( evento.keyCode == 13) {
			return document.formulario.ususenha.focus();
		};
	} else {
		return true;
	}
}

function controlar_foco_senha( evento ) {
	if ( window.event || evento.which ) {
		if ( evento.keyCode == 13) {
			return enviar_formulario();
		};
	} else {
		return true;
	}
}
</script>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<form method="POST"  name="formulario">
			<tr>
				<td align='right'  class="SubTituloDireita">Departamento:</td>
				<td>
					<?
					$sql = "SELECT depid as codigo, depsigla||' - '||depdsc as descricao FROM sisplan.departamento where depstatus = 'A' ORDER BY 2";
					$db->monta_combo("depid",$sql,"","&nbsp;",'','');
					?>
				</td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Tabela de Apoio:</td>
				<td>
					<?
					foreach ( $buscaTabela as $op => $k ){
						if( $_REQUEST['idTabela'] == $op )
							$compl = "selected";
						else 
							$compl = "";
						$output .= "<option value=\"".$op."\" ".$compl.">".$k['descricao']."</option>\n";
					}
					?>
					<select name="tabela" id="tabela" class="CampoEstilo" style="width:300px" onchange="recuperaDados(this.value)">
						<option value=""></option>
						<?=$output; ?>
					</select>
				</td>
			</tr>
			
			<tr>
				<td align='right'  class="SubTituloDireita">Usuário:</td>
				<td>
				<?=campo_texto('usucpf', 'S', 'S', '', 20, 14,'###.###.###-##','','','','','onkeypress="return controlar_foco_cpf( event )"')?>
					
				</td>
			</tr>
			
			<tr>
				<td align='right'  class="SubTituloDireita">Senha:</td>
				<td>
					<input type="password" name="ususenha" autocomplete="off" size="20" onkeypress="return controlar_foco_senha( event );" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"><br>
				</td>
			</tr>
			
			<tr bgcolor="#CCCCCC">
				<td></td>
				<td>
				<input type="button" name="btgerar" value="Gerar" onclick="validar()" class="botao">
				</td>
			</tr>
		</form>
	</table>