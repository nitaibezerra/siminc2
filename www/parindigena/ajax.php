<?php

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include("_constantes.php");
// atualiza ação do usuário no sistema
include APPRAIZ . "includes/registraracesso.php";

function verificaTipoPerfil( $usucpf )
{    
    global $db;
    $sql = "SELECT pflcod FROM seguranca.perfilusuario WHERE usucpf = '$usucpf'";
    $perfil = $db->pegaUm( $sql );
    return $perfil;    
}

function arrayPerfil(){
	global $db;
	
	$sql = sprintf("SELECT
						pu.pflcod
					FROM
						seguranca.perfilusuario pu
					INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 								 p.sisid  = 32
					WHERE
						pu.usucpf = '%s'
					ORDER BY
						p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

if(isset($_GET['tipo'])) {
	switch($_GET['tipo']) {
		
		case "carrega_obras":
		break;
		
		case 'carrega_grupos':
			$grupo = $db->carregar("SELECT 
										grpnome,
										grpid 
									FROM parindigena.grupoinformacao 
									WHERE grpid <> 5 
									ORDER BY grpid");
 			
			$retorno = trim($_GET['estuf']);
			
			if(($grupo != "") || ($grupo != NULL)) {
				for($i=0; $i < count($grupo); $i++) {
                    
                    $sql = "SELECT 
			                    DISTINCT 
			                    	c.covcod as codigo, 
			                    	c.covnumero as numero, 
			                    	cc.coivalorfinicial, 
			                    	cc.coivalorfcontinuada, 
			                    	cc.coivalormaterial
		                    FROM 
	                    		financeiro.convenio c									  
		                    INNER JOIN
		                    	parindigena.itemmonitoramento m ON c.covcod = m.covcod  
		                    INNER JOIN 
		                    	parindigena.convenioindigena cc ON c.covcod = cc.covcod
		                    WHERE
			                    c.covstatus = 'A' AND 
			                    m.grpid     = '{$grupo[$i]["grpid"]}' AND 
			                    m.itmstatus = 'A' AND
			                    cc.coitipo  = 'F' AND
			                    m.estuf     = '{$_GET['estuf']}'";
                    
                    $count = $db->carregar( $sql );
                    
                    if ( $count >   0 )
                    {
                        $vazio = 'F';
                    }
                    else
                    {
                        $vazio = 'T';
                    }

					$perfis = arrayPerfil();
				  
					if( in_array( PARIND_ADMINISTRADOR, $perfis ) || in_array( PARIND_SUPER_USUARIO, $perfis ) ){			
						$perfil = 1;			
					} else {
						$perfil = 0;
					}
                    
					$retorno .= '@'.trim($grupo[$i]["grpid"]).'&'.trim($grupo[$i]["grpnome"]).'&'.$vazio.'&'.$perfil;
				}
			}
			echo $retorno;
			break;
			
		case 'carrega_convenios':
			$dados = explode("_", $_GET['val']);
		
			$convenio = $db->carregar("SELECT DISTINCT 
											c.covcod as codigo, 
											c.covnumero as numero, 
											coalesce(ci.coivalorfinicial,0), 
											coalesce(ci.coivalorfcontinuada,0), 
											coalesce(ci.coivalormaterial,0)
										FROM parindigena.convenioindigena AS ci
										INNER join financeiro.convenio 			 c ON ci.covcod     = c.covcod									  
										INNER JOIN parindigena.itemmonitoramento m ON c.covcod      = m.covcod AND 
																						m.grpid     = '{$dados[1]}' AND 
																						m.itmstatus = 'A'
										WHERE 
											c.covstatus = 'A' AND 
											ci.coitipo  = 'F'
										AND c.estuf = '{$dados[0]}'"); 
			
//			$db->carregar("SELECT 
//                    DISTINCT c.covcod as codigo, c.covnumero as numero, cc.coivalorfinicial, cc.coivalorfcontinuada, cc.coivalormaterial
//                    FROM 
//                    parindigena.convenioindigena AS ci
//                    inner join 
//                    financeiro.convenio c ON ci.covcod = c.covcod									  
//                    INNER JOIN
//                    parindigena.itemmonitoramento m ON c.covcod = m.covcod  
//                    INNER JOIN 
//                    parindigena.convenioindigena cc ON  c.covcod = cc.covcod
//                                        
//                    WHERE
//                    c.covstatus = 'A'
//                    AND m.grpid = '{$dados[1]}'
//                    AND m.itmstatus = 'A'
//                    AND m.estuf = '{$dados[0]}'");           
                   
                    
			
			$retorno = trim($_GET['val']);
            
            $usucpf = $_SESSION['usucpf'];
            $perfil = verificaTipoPerfil( $usucpf );

                     
			if(($convenio != "") || ($convenio != NULL)) {
				for($i=0; $i < count($convenio); $i++) {
                    if( ( $convenio[$i]["coivalorfinicial"] == 0.00 ) || ( $convenio[$i]["coivalorfcontinuada"] == 0.00 ) || ( $convenio[$i]["coivalormaterial"] == 0.00 )  )
                    {
                        if ( ( PARIND_COORDENACAO == $perfil ) || ( PARIND_SUPER_USUARIO == $perfil ) )
                        {
                            $pendente = "P";
                        }
                        else
                        {
                             $pendente = "N";
                        }
                    }
                    else
                    {
                        $pendente = "N";
                    }       
					$retorno .= '@'.trim($convenio[$i]["codigo"]).'&'.trim($convenio[$i]["numero"]).'&'.$pendente;
				}
			}
			echo $retorno;
			break;
             
		case 'carrega_itens':
			$retorno = trim($_GET['val']);
			$dados = explode("_", $_GET['val']);
			
			// Carrega Itens de Formação Inicial.
			if($dados[1] == '1')
            {
				$formacaoinicial = $db->carregar("SELECT 
													distinct(fi.friid) as codigo, m.itmnome as descricao 
												FROM
													parindigena.formacaoinicial fi													
												INNER JOIN
													parindigena.itemmonitoramento m ON m.estuf  = '".$dados[0]."' AND 
																					   m.grpid  = ".$dados[1]." AND 
																					   m.covcod = '".$dados[2]."'													
												WHERE
													m.friid   is not null AND 
													fi.friid    = m.friid AND 
													m.itmstatus = 'A'");
				 
				if(($formacaoinicial != "") || ($formacaoinicial != NULL)) {
					for($i=0; $i < count($formacaoinicial); $i++) {
                        
                        $sql = "SELECT 
                            fe.fiedatainicial,
                            fe.fiedatafinal
                        from 
                            parindigena.formacaoinicialetapa as fe
                        WHERE 
                            fe.friid = '{$formacaoinicial[$i]["codigo"]}'
                        ORDER BY
                            fe.fiedatainicial";
                            
                        $count = $db->carregar( $sql );
                        if ( $count >   0 )
                        {
                            $vazio = 'F';
                        }
                        else
                        {
                            $vazio = 'T';
                        }       
            
						$retorno .= '@'.trim($formacaoinicial[$i]["codigo"]).'&'.trim($formacaoinicial[$i]["descricao"]).'&'.$vazio;
					}
				}
			}
			// Carrega itens de Formação Continuada.
			if($dados[1] == '2') 
            {
				$formacaocontinuada = $db->carregar("SELECT 
														distinct(fc.frcid) as codigo,m.itmnome as descricao 
													FROM
														parindigena.formacaocontinuada fc													
													INNER JOIN
														parindigena.itemmonitoramento m ON m.estuf = '".$dados[0]."' 
																				   AND m.grpid = ".$dados[1]." 
																				   AND m.covcod = '".$dados[2]."'	
													WHERE
														m.frcid is not null AND fc.frcid = m.frcid AND m.itmstatus = 'A' ");
                 
				if(($formacaocontinuada != "") || ($formacaocontinuada != NULL)) 
                {
					for($i=0; $i < count($formacaocontinuada); $i++)
                    {
                        
                        $sql = "SELECT 
                            fe.frcdatainicial,
                            fe.frcdatafim
                        FROM 
                            parindigena.formacaocontinuadaetapa as fe
                        
                        WHERE 
                            fe.frcid = '".$formacaocontinuada[$i]["codigo"]."'
                        ORDER BY
                            fe.frcdatainicial";
                        $count = $db->carregar( $sql );
                                    
                        if ( $count >   0 )
                        {
                            $vazio = 'F';
                        }
                        else
                        {
                            $vazio = 'T';
                        }       
                            
                        $retorno .= '@'.trim($formacaocontinuada[$i]["codigo"]).'&'.trim($formacaocontinuada[$i]["descricao"]).'&'.$vazio;
                    }
                }
			}  
			// Carrega itens de Material Didático.
			if($dados[1] == '3') 
            {
				$materialdidatico = $db->carregar("SELECT 
														distinct(md.madid) as codigo,m.itmnome as descricao 
													FROM
														parindigena.materialdidatico md													
													INNER JOIN
														parindigena.itemmonitoramento m ON m.estuf = '".$dados[0]."' 
																				   AND m.grpid = ".$dados[1]." 
																				   AND m.covcod = '".$dados[2]."'
													WHERE
														m.madid is not null AND md.madid = m.madid AND m.itmstatus = 'A'");
               
				
				if(($materialdidatico != "") || ($materialdidatico != NULL)) 
                {
					for($i=0; $i < count($materialdidatico); $i++) 
                    {
                        
                         
                        $sql = "SELECT 
                                    me.maedatainicial,
                                    me.maedatafinal,
                                    et.etpdescricao
                                FROM 
                                    parindigena.materialdidaticoetapa as me
                                INNER JOIN 
                                    parindigena.etapaproducao AS et
                                        ON me.etpid = et.etpid
                                WHERE 
                                    me.madid = '".$materialdidatico[$i]["codigo"]."'
                                ORDER BY
                                    me.maedatainicial";
                        
                        $count = $db->carregar( $sql );           
                                    
                        if ( $count >   0 )
                        {
                            $vazio = 'F';
                        }
                        else
                        {
                            $vazio = 'T';
                        }       
                            
                        $retorno .= '@'.trim($materialdidatico[$i]["codigo"]).'&'.trim($materialdidatico[$i]["descricao"]).'&'.$vazio;
                    }
                }
			}
			// Carrega itens de Obras.
			if($dados[1] == '4') 
            {
				$obra = $db->carregar(" SELECT 
											obras.obrid AS codigo,
											obras.obrdesc AS descricao,
											item.itmid
										FROM 
											obras.obrainfraestrutura obras
										INNER JOIN 
											parindigena.itemmonitoramento item ON obras.obrid = item.obrid
										WHERE 
											item.itmstatus  = 'A'
											AND item.estuf  = '".$dados[0]."'
											AND item.grpid = ".$dados[1]."
											AND item.covcod = '".$dados[2]."'");
                
				if(($obra != "") || ($obra != NULL)) 
                {
					for($i=0; $i < count($obra); $i++) 
                    {
                        
                        if ( $count >   0 )
                        {
                            $vazio = 'F';
                        }
                        else
                        {
                            $vazio = 'T';
                        }       
                            
                        //$retorno .= '@'.trim($obra[$i]["codigo"]).'&'.trim($obra[$i]["descricao"]).'&'.$vazio;
                        $retorno .= '@'.trim($obra[$i]["codigo"]).'&'.trim($obra[$i]["descricao"]).'&'.trim($obra[$i]["itmid"]).'&'.$vazio;
					}
				}
			}
			
			echo $retorno;
			break;
             
            case 'carrega_etapas':
			$dados = explode("_", $_GET['val']);
			
            if ( $dados[1] == '1' )
            {    
                $etapas = $db->carregar("SELECT 
                                            fe.fiedescricao,
                							fe.fiedatainicial,
                                            fe.fiedatafinal
                                        from 
                                            parindigena.formacaoinicialetapa as fe
                                        WHERE 
                                            fe.friid = '".$dados[3]."'
                                        ORDER BY
                                            fe.fiedatainicial");
                
                $retorno = trim($_GET['val']);
                
                if(($etapas != "") || ($etapas != NULL)) 
                {
                    for($i=0; $i < count($etapas); $i++) 
                    {
                        $retorno .= '@'.trim($etapas[$i]["fiedatainicial"]).'&'.trim($etapas[$i]["fiedatafinal"]).'&'.$i.'&'.trim($etapas[$i]["fiedescricao"]);
                    }
                }
            }
            
            if ( $dados[1] == '2' )
            {    
                $etapas = $db->carregar("SELECT 
                							fe.frcdescricao,
                                            fe.frcdatainicial,
                                            fe.frcdatafim
                                        FROM 
                                            parindigena.formacaocontinuadaetapa as fe
                                        
                                        WHERE 
                                            fe.frcid = '".$dados[3]."'
                                        ORDER BY
                                            fe.frcdatainicial");
                
                $retorno = trim($_GET['val']);
                
                if(($etapas != "") || ($etapas != NULL)) 
                {
                    for($i=0; $i < count($etapas); $i++) 
                    {
                        $retorno .= '@'.trim($etapas[$i]["frcdatainicial"]).'&'.trim($etapas[$i]["frcdatafim"]).'&'.$i.'&'.trim($etapas[$i]["frcdescricao"]);
                    }
                }
            }
            
            if ( $dados[1] == '3' )
            {    
                $etapas = $db->carregar("SELECT 
                                            me.maedatainicial,
                                            me.maedatafinal,
                                            et.etpdescricao
                                        FROM 
                                            parindigena.materialdidaticoetapa as me
                                        INNER JOIN 
                                            parindigena.etapaproducao AS et
                                                ON me.etpid = et.etpid
                                        WHERE 
                                            me.madid = '".$dados[3]."'
                                        ORDER BY
                                            me.maedatainicial");
                
                $retorno = trim($_GET['val']);
                
                if(($etapas != "") || ($etapas != NULL))
                {
                    for($i=0; $i < count($etapas); $i++) 
                    {
                        $retorno .= '@'.trim($etapas[$i]["maedatainicial"]).'&'.trim($etapas[$i]["maedatafinal"]).'&'.trim($etapas[$i]["etpdescricao"]);
                    }
                }
            }
            
			echo $retorno;
			break;
             
		case 'carrega_barra_execucao':
			$dados = explode("_", trim($_GET['val']));

			$item = "";
			// Formação Inicial
			if($dados[1] == 1)
				$item = "m.friid = ".$dados[3]." AND ";
			// Formação Continuada
			if($dados[1] == 2)
				$item = "m.frcid = ".$dados[3]." AND ";
			// Material Didático
			if($dados[1] == 3)
				$item = "m.madid = ".$dados[3]." AND ";
			// Obras
			if($dados[1] == 4)
				$item = "m.obrid = ".$dados[3]." AND ";
				
			$situacao = $db->carregar("SELECT
										m.esaid,m.itmporcentoexec,ea.esadescricao
									  FROM 
									  	parindigena.itemmonitoramento m
									  INNER JOIN
									  	pde.estadoatividade ea ON ea.esaid = m.esaid
									  WHERE
									  	m.estuf = '".$dados[0]."' AND
									  	m.grpid = ".$dados[1]." AND
									  	m.covcod = '".$dados[2]."' AND
									  	".$item."
									  	m.itmstatus = 'A'");
			
			switch($situacao[0]["esaid"]) 
            {
				// Não iniciado
				case 1:
					$cor_texto = '#909090';
					$cor_barra = '#bbbbbb';
					$cor_sombra = '#efefef';
					break;
				// Em andamento
				case 2:
					$cor_texto = '#209020';
					$cor_barra = '#339933';
					$cor_sombra = '#dcffdc';
					break;
				// Suspenso
				case 3:
					$cor_texto = '#aa9020';
					$cor_barra = '#bba131';
					$cor_sombra = '#feffbf';
					break;
				// Cancelado
				case 4:
					$cor_texto = '#aa2020';
					$cor_barra = '#cc3333';
					$cor_sombra = '#ffe7e7';
					break;
				// Concluído
				case 5:
					$cor_texto = '#2020aa';
					$cor_barra = '#3333cc';
					$cor_sombra = '#d4e7ff';
					break;
			}
			
			$retorno = sprintf(
			'<span style="color:%s; font-size:10px;">%s</span>' .
			'<div style="text-align:left; margin-left:5px; padding:1px 0 1px 0; height:6px; max-height:6px; width:75px; border:1px solid #888888; background-color:%s;" title="%d%%">' .
			'<div style="font-size:4px; width:%d%%; height:6px; max-height:6px; background-color:%s;">' .
			'</div>'.
			'</div>',
			$cor_texto,
			$situacao[0]["esadescricao"],
			$cor_sombra,
			$situacao[0]["itmporcentoexec"],
			$situacao[0]["itmporcentoexec"],
			$cor_barra
			);
			
			$retorno .= '@@'.$situacao[0]["esaid"].'@@'.$situacao[0]["itmporcentoexec"];
			echo $retorno;
			break;
			
		case 'atualiza_barra_status':
			$dados = explode("_", trim($_GET['id']));
			
			$item = "";
			// Formação Inicial
			if($dados[1] == 1)
				$item = "friid = ".$dados[3]." AND ";
			// Formação Continuada
			if($dados[1] == 2)
				$item = "frcid = ".$dados[3]." AND ";
			// Material Didático
			if($dados[1] == 3)
				$item = "madid = ".$dados[3]." AND ";
			// Obras
			if($dados[1] == 4)
				$item = "obrid = ".$dados[3]." AND ";
				
			$db->executar("UPDATE
							parindigena.itemmonitoramento
						   SET
						   	itmporcentoexec = ".trim($_GET['percentual']).",
						   	esaid = ".trim($_GET['codstatus'])."
						   WHERE
						   	estuf = '".$dados[0]."' AND
						  	grpid = ".$dados[1]." AND
						  	covcod = '".$dados[2]."' AND
						  	".$item."
						  	itmstatus = 'A'");
			
			$retorno = $db->commit();
			echo $retorno;
			break;
			
		case 'carrega_data':
			$dados = explode("_", trim($_GET['val']));
			
			$item = "";
			// Formação Inicial
			if($dados[1] == 1)
				$item = "m.friid = ".$dados[3]." AND ";
			// Formação Continuada
			if($dados[1] == 2)
				$item = "m.frcid = ".$dados[3]." AND ";
			// Material Didático
			if($dados[1] == 3)
				$item = "m.madid = ".$dados[3]." AND ";
			// Obras
			if($dados[1] == 4)
				$item = "m.obrid = ".$dados[3]." AND ";
				
			$data = $db->carregar("SELECT 
									m.itmdatainicio, m.itmdatafim
								  FROM 
								  	parindigena.itemmonitoramento m
								  WHERE
								  	m.estuf = '".$dados[0]."' AND
								  	m.grpid = ".$dados[1]." AND
								  	m.covcod = '".$dados[2]."' AND
								  	".$item."
								  	m.itmstatus = 'A'");
			
			if(($data[0]["itmdatainicio"] != NULL)&&($data[0]["itmdatainicio"] != ""))
				$inicio = strftime("%d/%m/%Y",strtotime($data[0]["itmdatainicio"]));
			else
				$inicio = "";
				
			if(($data[0]["itmdatafim"] != NULL)&&($data[0]["itmdatafim"] != ""))
				$termino = strftime("%d/%m/%Y",strtotime($data[0]["itmdatafim"]));
			else
				$termino = "";
				
			$retorno = $inicio."@".$termino;	
			
			echo $retorno;
			break;
			
		case 'atualiza_data_item':
			$dados = explode("_", trim($_GET['id']));
			
			$item = "";
			// Formação Inicial
			if($dados[1] == 1)
				$item = "friid = ".$dados[3]." AND ";
			// Formação Continuada
			if($dados[1] == 2)
				$item = "frcid = ".$dados[3]." AND ";
			// Material Didático
			if($dados[1] == 3)
				$item = "madid = ".$dados[3]." AND ";
			// Obras
			if($dados[1] == 4)
				$item = "obrid = ".$dados[3]." AND ";
			
			$db->executar("UPDATE 
							parindigena.itemmonitoramento
						   SET
						   	".trim($_GET['data_alterada'])." = '".trim($_GET['nova_data'])."'						   	
						   WHERE
						   	estuf = '".$dados[0]."' AND
						  	grpid = ".$dados[1]." AND
						  	covcod = '".$dados[2]."' AND
						  	".$item."
						  	itmstatus = 'A'");
			
			$retorno = $db->commit();
			echo $retorno;
			break;
	}
} 
?>