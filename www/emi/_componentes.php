<?php

class emi{
	
	public function __construct(){
		
		global $db;
		$this->db = $db;
		
	}
	
	public function trataDados( $dados = array() ){
		
		if ( is_array( $dados ) ){
			foreach ( $dados as $campo=>$valor ){
				if ( !is_numeric( $valor ) ){
					$dados[$campo] = !empty( $valor ) ? "'" . pg_escape_string( trim( $valor ) ) . "'" : "''";
				}else{
					$valor = str_replace(".", "", $valor);
					$valor = str_replace(",", ".", $valor);
					$dados[$campo] = !empty( $valor ) ? $valor : "NULL";
				}
			}
		}else{
			if ( !is_numeric( $dados ) ){
				$dados = !empty( $dados ) ? "'" . pg_escape_string( trim( $dados ) ) . "'" : "''";
			}else{
				$valor = str_replace(".", "", $valor);
				$valor = str_replace(",", ".", $valor);
				$dados = !empty( $dados ) ? $dados : "NULL";
			}
		}
		
		return $dados;
		
	}
	
	public function listaEstadoSecretaria( $tipo = "normal", $carga = "" ){
		
		switch( $tipo ){
			
			case "normal":
				
				$sql = "SELECT
							'<center>
								<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || wd.esdid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || wd.esdid || ' );abreconteudo(\'emi.php?modulo=principal/statusSecretaria&acao=A&subAcao=gravarCarga&carga=' || wd.esdid || '&params=\' + params, ' || wd.esdid || ');\"/>
								</center>' as acao,
							CASE WHEN em.docid is null THEN '' ELSE we.esddsc END as descricao,
							count(emeid) as total,
							'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || wd.esdid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 1px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
						FROM
							workflow.documento wd
						LEFT JOIN
							workflow.estadodocumento we ON we.esdid = wd.esdid
						LEFT JOIN
							emi.ementidade em ON em.docid = wd.docid
						WHERE 
							wd.tpdid = " . EMI_TIPO_DOCUMENTO . " AND emestatus = 'A' AND emiexercicio = '{$_SESSION['exercicio']}'
						GROUP BY
							descricao, wd.esdid";
		
				$cabecalho = array( "Ação", "Estado Atual", "Qtd de Secretarias" , "");
				$this->db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
					
			break;
			
			case "carga":
				
				$sql = "SELECT DISTINCT
							'<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abreDadosSec( \''|| ed.estuf ||'\', \'inicio\' );\"></center>' as acao,
							entnome as nome
						FROM
							entidade.entidade ee
						INNER JOIN
							entidade.endereco ed ON ed.entid = ee.entid
						INNER JOIN
							emi.ementidade em ON em.entid = ee.entid
						INNER JOIN
							workflow.documento wd ON wd.docid = em.docid
						INNER JOIN
							workflow.estadodocumento we ON we.esdid = wd.esdid
						WHERE
							we.esdid = {$carga} AND trim(ed.estuf) != '' AND emiexercicio = '{$_SESSION['exercicio']}'
						ORDER BY
							entnome";
				
				$cabecalho = array( "Ação", "Secretaria" );
				$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
				
				
			break;
			
		}
				
	}
	
	public function listaEstados( $tipo = "inicio" ){
		
		if( emiPossuiPerfil( EMI_PERFIL_CADASTRADOR ) && !$this->db->testa_superuser() ){
		
			$sql = "SELECT
						'<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abreDadosSec( \''|| te.estuf ||'\', \'{$tipo}\' );\"></center>' as codigo,
						estdescricao as descricao
					FROM
						territorios.estado te
					INNER JOIN
						emi.usuarioresponsabilidade eu ON eu.estuf = te.estuf
					WHERE
						rpustatus = 'A' AND usucpf = '{$_SESSION["usucpf"]}'
					ORDER BY
						te.estuf";
			
			$ufs = $this->db->carregar( $sql );
			
			if( $ufs ){
				
				$cabecalho = array( "Ação", "Estado" );
				$this->db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
					
			}else{
				
				print "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
						   <tr>
						       <td style='color:red;' align='center'>
						           Usuários sem permissões para visualizar os estados. <br/>
						           Favor entrar em contato com o Administrador do sistema.
						       </td>
						    </tr>
					   </table>";
				
			}
						
		}else{
			
			$sql = "SELECT
						'<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abreDadosSec( \''|| estuf ||'\', \'{$tipo}\' );\"></center>' as codigo,
						estdescricao as descricao
					FROM
						territorios.estado
					ORDER BY
						estuf";
			
			$cabecalho = array( "Ação", "Estado" );
			$this->db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
			
		}
		
	}
	
	public function listaEscolasPorEstado( $uf ){
			
		$sql = "select 
					'<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abreRelatorioDespesas(\'' || emi.entid || '\');\"></center>' as acao,
					entnome 
				from 
					emi.ementidade emi
				inner join
					entidade.entidade ent ON emi.entid = ent.entid
				where 
					estuf = '$uf'
				and
					emeidpai is not null
				and
					entstatus = 'A'
				and
					emestatus = 'A'
				and
					emiexercicio = '{$_SESSION['exercicio']}'
				and
					tppid = 1
				order by
					entnome";
		$cabecalho = array( "Ação", "Escola" );
		$this->db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
			
	}

	public function montaCabecalho( $id, $tipo = "secretaria" ){
		
		if( !$_SESSION["emi"]["emeidPai"] ){

				print "<script>
				  		   alert('A sessão expirou. Selecione o estado novamente!');
						   history.back(-1);
					  </script>";
				die;
			
		}
		
		switch ( $tipo ){
			
			case "macrocampo":
				
				$sql = "SELECT
							mcpdsc,
							papcaoatividade,
							papmeta
						FROM
							emi.emgap em
						INNER JOIN
							emi.macrocampo mac ON mac.mcpid = em.mcpid
						WHERE
							em.papid = {$id}";
				
				$dados = $this->db->pegaLinha( $sql );
				
				print "<table class='tabela' bgcolor='#f5f5f5' height='100%' cellspacing='1' cellpadding='3' align='center'>"
					. "	   <tr>"
					. "	       <td width='190px' class='subtitulodireita'>Macrocampo:</td>"
					. "	       <td>"
					.  			   $dados["mcpdsc"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Ação / Atividade:</td>"
					. "	       <td>"
					.  			   $dados["papcaoatividade"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Meta:</td>"
					. "	       <td>"
					.  			   $dados["papmeta"]
					. "	       </td>"
					. "	   </tr>"
					. "</table>";
					
			break;
			
			case "secretaria":
				
				$sql = "SELECT
							entnome as nome,
							ed.estuf as uf,
							mundescricao as municipio
						FROM
							entidade.entidade ee
						INNER JOIN
							emi.ementidade em ON em.entid = ee.entid
						INNER JOIN
							entidade.endereco ed ON ed.entid = ee.entid
						INNER JOIN
							territorios.municipio tm ON tm.muncod = ed.muncod
						WHERE
							em.emeid = {$id}";
				
				$dados = $this->db->pegaLinha( $sql );
				
				print "<table class='tabela' bgcolor='#f5f5f5' height='100%' cellspacing='1' cellpadding='3' align='center'>"
					. "	   <tr>"
					. "	       <td width='190px' class='subtitulodireita'>Nome da Secretaria:</td>"
					. "	       <td>"
					.  			   $dados["nome"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Município / UF:</td>"
					. "	       <td>"
					.  			   $dados["municipio"] . " / " . $dados["uf"]
					. "	       </td>"
					. "	   </tr>"
					. "</table>";
					
			break;
			
			case "escola":
				
				$emeidPai = $_SESSION["emi"]["emeidPai"];
				
				$sql = "SELECT entid FROM emi.ementidade WHERE emeid = {$emeidPai}";
				$entidPai = $this->db->pegaUm( $sql );
				
				$sql = "SELECT entnome as nome FROM entidade.entidade where entid = {$entidPai}";
				$nomePai = $this->db->pegaUm( $sql );
				
				$sql = "SELECT
							ee.entcodent || ' - ' || entnome as nome,
							ed.estuf as uf,
							mundescricao as municipio
						FROM
							entidade.entidade ee
						INNER JOIN
							emi.ementidade em ON em.entid = ee.entid
						INNER JOIN
							entidade.endereco ed ON ed.entid = ee.entid
						INNER JOIN
							territorios.municipio tm ON tm.muncod = ed.muncod
						WHERE
							em.emeid = {$id}";
				
				$dados = $this->db->pegaLinha( $sql );
				
				print "<table class='tabela' bgcolor='#f5f5f5' height='100%' cellspacing='1' cellpadding='3' align='center'>"
					. "	   <tr>"
					. "	       <td width='190px' class='subtitulodireita'>Nome da Secretaria:</td>"
					. "	       <td>"
					.  			   $nomePai
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td width='190px' class='subtitulodireita'>Nome da Escola:</td>"
					. "	       <td>"
					.  			   $dados["nome"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Município / UF:</td>"
					. "	       <td>"
					.  			   $dados["municipio"] . " / " . $dados["uf"]
					. "	       </td>"
					. "	   </tr>"
					. "</table>";
					
			break;
			
			case "componente":
				
				$sql = "SELECT
							dimcod || '. ' || dimdsc as dimensao,
							comcod || '. ' || comdsc as componente,
							laccod || '. ' || lacdsc as linhaacao
						FROM
							emi.emcomponentes ec
						INNER JOIN
							emi.emlinhaacao el ON el.lacid = ec.lacid
						INNER JOIN
							emi.emdimensao emd ON emd.emdid = el.emdid
						INNER JOIN
							cte.dimensao cd ON cd.dimid = emd.dimid
						WHERE
							ec.comid = '{$id}'";
				
				$dados = $this->db->pegaLinha( $sql );
				
				print "<table class='tabela' bgcolor='#f5f5f5' height='' cellspacing='1' cellpadding='3' align='center'>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Dimensão / PAR:</td>"
					. "	       <td>"
					.  			   $dados["dimensao"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Linha de Ação:</td>"
					. "	       <td>"
					.  			   $dados["linhaacao"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Componente:</td>"
					. "	       <td>"
					.  			   $dados["componente"]
					. "	       </td>"
					. "	   </tr>"
					. "</table>";
				
			break;
			
		case "matrizsecretaria":
				
				$emeidPai = $_SESSION["emi"]["emeidPai"];
				
				$sql = "SELECT entid FROM emi.ementidade WHERE emeid = {$emeidPai}";
				$entidPai = $this->db->pegaUm( $sql );
				
				$sql = "SELECT entnome as nome FROM entidade.entidade where entid = {$entidPai}";
				$nomePai = $this->db->pegaUm( $sql );
				
				$sql = "SELECT
							entnome as nome,
							ed.estuf as uf,
							mundescricao as municipio,
							dimcod || '. ' || dimdsc as dimensao,
							comcod || '. ' || comdsc as componente,
							laccod || '. ' || lacdsc as linhaacao,
							papcaoatividade as atividade,
							papmeta as meta
						FROM
							entidade.entidade ee
						INNER JOIN
							emi.ementidade em ON em.entid = ee.entid
						INNER JOIN
							emi.empap ep ON ep.emeid = em.emeid
						INNER JOIN
							emi.emcomponentes ec ON ec.comid = ep.comid
						INNER JOIN
							emi.emlinhaacao el ON el.lacid = ec.lacid
						INNER JOIN
							emi.emdimensao emd ON emd.emdid = el.emdid
						INNER JOIN
							cte.dimensao cd ON cd.dimid = emd.dimid
						INNER JOIN
							entidade.endereco ed ON ed.entid = ee.entid
						INNER JOIN
							territorios.municipio tm ON tm.muncod = ed.muncod
						WHERE
							ep.papid = {$id}";
				
				$dados = $this->db->pegaLinha( $sql );
				
				print "<table class='tabela' bgcolor='#f5f5f5' height='100%' cellspacing='1' cellpadding='3' align='center'>"
					. "	   <tr>"
					. "	       <td width='190px' class='subtitulodireita'>Nome da Secretaria:</td>"
					. "	       <td>"
					.  			   $nomePai
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Município / UF:</td>"
					. "	       <td>"
					.  			   $dados["municipio"] . " / " . $dados["uf"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulocentro' colspan='2'>Dados do PAP</td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Dimensão / PAR:</td>"
					. "	       <td>"
					.  			   $dados["dimensao"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Linha de Ação:</td>"
					. "	       <td>"
					.  			   $dados["linhaacao"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Componente:</td>"
					. "	       <td>"
					.  			   $dados["componente"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Ação/Atividade:</td>"
					. "	       <td>"
					.  			   $dados["atividade"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Meta:</td>"
					. "	       <td>"
					.  			   $dados["meta"]
					. "	       </td>"
					. "	   </tr>"
					. "</table>";
				
				
			break;
			
			case "matriz":
				
				$emeidPai = $_SESSION["emi"]["emeidPai"];
				
				$sql = "SELECT entid FROM emi.ementidade WHERE emeid = {$emeidPai}";
				$entidPai = $this->db->pegaUm( $sql );
				
				$sql = "SELECT entnome as nome FROM entidade.entidade where entid = {$entidPai}";
				$nomePai = $this->db->pegaUm( $sql );
				
				$sql = "SELECT
							entnome as nome,
							ed.estuf as uf,
							mundescricao as municipio,
							dimcod || '. ' || dimdsc as dimensao,
							comcod || '. ' || comdsc as componente,
							laccod || '. ' || lacdsc as linhaacao,
							papcaoatividade as atividade,
							papmeta as meta
						FROM
							entidade.entidade ee
						INNER JOIN
							emi.ementidade em ON em.entid = ee.entid
						INNER JOIN
							emi.empap ep ON ep.emeid = em.emeid
						INNER JOIN
							emi.emcomponentes ec ON ec.comid = ep.comid
						INNER JOIN
							emi.emlinhaacao el ON el.lacid = ec.lacid
						INNER JOIN
							emi.emdimensao emd ON emd.emdid = el.emdid
						INNER JOIN
							cte.dimensao cd ON cd.dimid = emd.dimid
						INNER JOIN
							entidade.endereco ed ON ed.entid = ee.entid
						INNER JOIN
							territorios.municipio tm ON tm.muncod = ed.muncod
						WHERE
							ep.papid = {$id}";
				
				$dados = $this->db->pegaLinha( $sql );
				
				print "<table class='tabela' bgcolor='#f5f5f5' height='100%' cellspacing='1' cellpadding='3' align='center'>"
					. "	   <tr>"
					. "	       <td width='190px' class='subtitulodireita'>Nome da Secretaria:</td>"
					. "	       <td>"
					.  			   $nomePai
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td width='190px' class='subtitulodireita'>Nome da Escola:</td>"
					. "	       <td>"
					.  			   $dados["nome"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Município / UF:</td>"
					. "	       <td>"
					.  			   $dados["municipio"] . " / " . $dados["uf"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulocentro' colspan='2'>Dados do PAP</td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Dimensão / PAR:</td>"
					. "	       <td>"
					.  			   $dados["dimensao"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Linha de Ação:</td>"
					. "	       <td>"
					.  			   $dados["linhaacao"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Componente:</td>"
					. "	       <td>"
					.  			   $dados["componente"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Ação/Atividade:</td>"
					. "	       <td>"
					.  			   $dados["atividade"]
					. "	       </td>"
					. "	   </tr>"
					. "	   <tr>"
					. "	       <td class='subtitulodireita'>Meta:</td>"
					. "	       <td>"
					.  			   $dados["meta"]
					. "	       </td>"
					. "	   </tr>"
					. "</table>";
				
				
			break;
			
		}
		
		
		
	}
	
	public function InsereEntidade( $dados ){
		
		$sql = "SELECT 
					emeid 
				FROM
					emi.ementidade  
				WHERE 
					entid = {$dados["id"]} AND
					emestatus = 'A'";
					
		
		$emeid = $this->db->pegaUm( $sql );
		
		if ( !$emeid ){
			
			$sql = "INSERT INTO emi.ementidade ( entid, tppid, emestatus , emiexercicio) VALUES ( {$dados["id"]}, " . EMI_TIPO_ENTIDADE_SEC. ",  'A', '{$_SESSION['exercicio']}') returning emeid";
			$emeid = $this->db->pegaUm( $sql );
			$this->db->commit();
			
			$_SESSION["emi"]["emeidPai"] = $emeid;
			
		}else{
			
			$_SESSION["emi"]["emeidPai"] = $emeid;
			
		}
		
	}
	
	public function montaArvore( $dados ){
		
		print "<script type=\"text/javascript\">"
			. "    arvore = new dTree( 'arvore' );"
			. "    arvore.config.folderLinks = true;"
			. "    arvore.config.useIcons    = true;"
			. "    arvore.config.useCookies  = true;"
			. "    arvore.add('1','-1','{$_SESSION["emi"]["nomepai"]}','');"
			. "    arvore.add('2','1','Ensino Médio Inovador','');";
		
		$coordenador = emiBuscaDadosCoordenador( $_SESSION["emi"]["emeidPai"] );
		
		if ( !$coordenador ){
			print "    arvore.add('3','2', 'Selecionar Coordenador','javascript:selecionaCoordenador(\'\');');";	
		} else{
			print "    arvore.add('3','2', '{$coordenador["entnome"]}', 'javascript:selecionaCoordenador({$coordenador["entid"]});','', '', '../imagens/check_p.gif');";
		}
		
		$arquivo = emiBuscaDadosArquivo( $_SESSION["emi"]["emeidPai"] );
		
		if( !$arquivo ) {
			print "    arvore.add('4','2','Inserir Análise Situacional','javascript:envirarFormulario({$_SESSION["emi"]["emeidPai"]}, \'secretaria\');');";
		} else{
			print "    arvore.add('4','2','Formulário enviado','javascript:envirarFormulario({$_SESSION["emi"]["emeidPai"]}, \'secretaria\');', '', '', '../imagens/check_p.gif');";
		}
		
//		$papsSec = emiBuscaPapSecretaria( $_SESSION["emi"]["emeidPai"] );
//		
//		if ( !$papsSec ){
//			print "    arvore.add('5','2','PAPS {$_SESSION['exercicio']}','javascript:pap({$_SESSION["emi"]["emeidPai"]}, " . EMI_TIPO_ENTIDADE_SEC . ");');";	
//		}else{
//			print "    arvore.add('5','2','PAPS {$_SESSION['exercicio']}','javascript:pap({$_SESSION["emi"]["emeidPai"]}, " . EMI_TIPO_ENTIDADE_SEC . ");', '', '', '../imagens/check_p.gif');";
//		}
			
			
		$escolas = emiBuscaEscolasCadastradas( $_SESSION["emi"]["emeidPai"] );
		if ( !$escolas ){
			
			print "    arvore.add('6','2','Selecionar Escolas','javascript:selecionaEscolas();');";
			
		}else{
			
			print "    arvore.add('6','2','Escolas','javascript:selecionaEscolas();');";
			
			$itemArvore = 6;
			
			for( $i = 0; $i < count($escolas); $i++ ){
				
				$itemArvore  = $itemArvore + 1;
				$itemArvore2 = $itemArvore + 1;
				$itemArvore3 = $itemArvore2 + 1;
				
				/*
				 * Alteração feita por Felipe Carvalho
				 * 25/11/2009
				 * Alguma escolas tem apóstrofe em suas descrições (campo 'entnome'), adicionado a função addslashes para correção.
				 */
				// Verifica se tem alguma crítica feita para as atividades/ações da escola, que possui validação negativa

				$entNomeEscola = (string) emiVerificaValidacaoCritica( $escolas[$i]["emeid"] );
				
				if( $entNomeEscola )
					$nomeEscola = "'".addslashes($escolas[$i]["entnome"])."'";

				if( $entNomeEscola == 'observacao' )
					$nomeEscola = "'<font color=\"orange\">".addslashes($escolas[$i]["entnome"])."</font>'";

				if( !$entNomeEscola )
					$nomeEscola = "'<font color=\"red\">".addslashes($escolas[$i]["entnome"])."</font>'";
				
				
				print "    arvore.add('e_{$escolas[$i]["emeid"]}','6', {$nomeEscola}, 'javascript:dadosEscolas({$escolas[$i]["emeid"]});');";

				$papsEsc = emiBuscaPap( $escolas[$i]["emeid"] );
				
				if( $_SESSION['exercicio'] != '2012' ){
					if( !$papsEsc ){
						print "    arvore.add('{$itemArvore2}','e_{$escolas[$i]["emeid"]}','PAP {$_SESSION['exercicio']}', 'javascript:pap({$escolas[$i]["emeid"]}, " . EMI_TIPO_ENTIDADE_ESCOLA . ");');";
					}else{
						print "    arvore.add('{$itemArvore2}','e_{$escolas[$i]["emeid"]}','PAP {$_SESSION['exercicio']}', 'javascript:pap({$escolas[$i]["emeid"]}, " . EMI_TIPO_ENTIDADE_ESCOLA . ");', '', '', '../imagens/check_p.gif');";
					}
				}
				
				$gapsEsc = emiBuscaGap( $escolas[$i]["emeid"] );
				
				if( !$gapsEsc ){
					print "    arvore.add('{$itemArvore2}','e_{$escolas[$i]["emeid"]}','PRC {$_SESSION['exercicio']}', 'javascript:gap({$escolas[$i]["emeid"]}, " . EMI_TIPO_ENTIDADE_ESCOLA . ");');";
				}else{
					print "    arvore.add('{$itemArvore2}','e_{$escolas[$i]["emeid"]}','PRC {$_SESSION['exercicio']}', 'javascript:gap({$escolas[$i]["emeid"]}, " . EMI_TIPO_ENTIDADE_ESCOLA . ");', '', '', '../imagens/check_p.gif');";
				}
				
				if( $_SESSION['exercicio'] != '2012' ){
					$arquivoEscola = emiBuscaDadosArquivo( $escolas[$i]["emeid"] );
					if( !$arquivoEscola ){
						print "    arvore.add('{$itemArvore3}','e_{$escolas[$i]["emeid"]}','Inserir Análise Situacional', 'javascript:envirarFormulario({$escolas[$i]["emeid"]}, \'escola\');');";
					}else{
						print "    arvore.add('{$itemArvore3}','e_{$escolas[$i]["emeid"]}','Formulário enviado', 'javascript:envirarFormulario({$escolas[$i]["emeid"]}, \'escola\');', '', '', '../imagens/check_p.gif');";
					}
				}
				
			} 
			
		}

		print "    var elemento = document.getElementById('_arvore');"
			. "    elemento.innerHTML = arvore;"
			. "</script>";
		
	}
	
	public function insereResponsavel( $entid ){
		
		$sql = "SELECT 
					er.rspid 
				FROM 
					emi.emresponsavel er 
				INNER JOIN 
					emi.ementidade em ON er.rspid = em.rspid
				WHERE
					emeid = {$_SESSION["emi"]["emeidPai"]}";
		
		$rspid = $this->db->pegaUm( $sql );
		
		if ( $rspid ){
			
			$sql = "UPDATE emi.emresponsavel SET entid = {$entid} WHERE rspid = {$rspid}";
			$this->db->executar( $sql );
			
		}else{
			
			$sql = "INSERT INTO emi.emresponsavel ( entid, rspstatus ) VALUES( {$entid}, 'A' ) returning rspid";
			$rspid = $this->db->pegaUm( $sql );
			
			$sql = "UPDATE emi.ementidade SET rspid = {$rspid} WHERE emeid = {$_SESSION["emi"]["emeidPai"]}";
			$this->db->executar( $sql );
			
		}
		
		$_SESSION["emi"]["entidResponsavel"] = $entid;
		
		$this->db->commit();
		$this->db->sucesso( 'principal/cadastraCoordenador' );
	}

	public function enviaFormulario( $dados ){
		
		// obtém o arquivo
		$arquivo = $_FILES['arquivo'];
		
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
			echo "<script>
					alert('Arquivo não enviado');
					window.location='emi.php?modulo=principal/uploadFormulario&acao={$dados["acao"]}';
				  </script>";
			exit;
		}
		
		// BUG DO IE
		// O type do arquivo vem como image/pjpeg
		if($arquivo["type"] == 'image/pjpeg') {
			$arquivo["type"] = 'image/jpeg';
		}
		
		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
		values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$dados["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
		$arqid = $this->db->pegaUm($sql);
		
		$dettipo = !$dados["dettipo"]  ? "P" : $dados["dettipo"];

		$sql = "UPDATE emi.detalheentidade SET arqid='".$arqid."' WHERE emeid='".$dados["emeid"]."' and dettipo = '$dettipo'";
		$this->db->executar($sql);
		
		if(!is_dir('../../arquivos/emi')) {
			mkdir(APPRAIZ.'/arquivos/emi', 0777);
		}

		if(!is_dir('../../arquivos/emi/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/emi/'.floor($arqid/1000), 0777);
		}
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
		
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			
			$this->db->rollback();
			
			echo "<script>
					alert('Problemas no envio do arquivo.');
					history.back(-1);
				  </script>";
			exit;
			
		}

		$this->db->commit();
		$this->db->sucesso("principal/arvoreSecretaria");
		
	}
	
	public function DownloadArquivo( $dados ){
		
		ob_clean();
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$dados['arqid'];
		$arquivo = $this->db->pegaLinha($sql);
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
		if ( !is_file( $caminho ) ) {
			$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
		}
		$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
		header( 'Content-type: '. $arquivo['arqtipo'] );
		header( 'Content-Disposition: attachment; filename='.$filename);
		readfile( $caminho );
		exit;
		
	}
	
	public function salvarParecer( $dados ){
		
		$sql = "UPDATE emi.emparecer SET prcstatus = 'I' WHERE emeid = {$dados["emeid"]}";
		$this->db->executar( $sql );
		
		$sql = "INSERT INTO emi.emparecer ( prcdataparecer, prcparecer, emeid, prcstatus, usucpf )  
								   VALUES ( 'now', '{$dados["prcparecer"]}', {$dados["emeid"]}, 'A', '{$_SESSION["usucpf"]}')";
		
		$this->db->executar( $sql );
		$this->db->commit();
		$this->db->sucesso( "principal/parecer" );
	}
	
	public function alteraQtdEscolas( $dados ){
		
		$sql = "UPDATE emi.ementidade SET emeqtdescolas = {$dados["emeqtdescolas"]} WHERE emeid = {$dados["emeid"]}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		$this->db->sucesso( "principal/alterarQtdEscolas" );
		
	}
	
	public function removeFormulario( $dados ) {
		
		$emeid 	 = $dados["acao"] == 'A' ? $_SESSION["emi"]["emeidPai"] : $_SESSION["emi"]["emeidEscola"];
		$dettipo = !$dados["dettipo"]  ? "P" : $dados["dettipo"];
		
		$sql = "UPDATE emi.detalheentidade SET arqid=NULL WHERE emeid = {$emeid} and dettipo = '$dettipo' ";
		$this->db->executar($sql);
		
		$this->db->commit();
		$this->db->sucesso("principal/arvoreSecretaria");
		
	}
	
	public function insereEscolas( $dados ){
		
		/*
		 * Escolas que podem ser excluidas da lista
		 *  - Somente se não tiver PAPs e não tiver submetido arquivo
		 */
		
		$sql = "SELECT emeid FROM emi.ementidade WHERE emiexercicio = '{$_SESSION['exercicio']}' AND emeidpai='".$_SESSION["emi"]["emeidPai"]."'";
		$escolasSemEstrutura = $this->db->carregar($sql);
		
		if($escolasSemEstrutura[0]) {
			foreach($escolasSemEstrutura as $eme){
				$apaga  = true;
				$sql = "SELECT dt.arqid FROM emi.ementidade em INNER JOIN emi.detalheentidade dt ON dt.emeid = em.emeid WHERE emiexercicio = '{$_SESSION['exercicio']}'  and em.emeid='".$eme['emeid']."'";
				$arqid = $this->db->pegaUm($sql);
				if($arqid) $apaga = false; 
				$sql = "SELECT COUNT(ep.papid) AS num 
					    FROM emi.empap ep WHERE papexercicio = '{$_SESSION['exercicio']}' and emeid='".$eme['emeid']."' AND papstatus='A'
						GROUP BY ep.emeid";
				$num = $this->db->pegaUm($sql);
				if($num > 0) $apaga = false;
				$sql = "SELECT COUNT(ep.papid) AS num 
					    FROM emi.emgap ep WHERE papexercicio = '{$_SESSION['exercicio']}' and emeid='".$eme['emeid']."' AND papstatus='A'
						GROUP BY ep.emeid";
				$num2 = $this->db->pegaUm($sql);
				if($num2 > 0) $apaga = false;
				
				if($apaga) {				
					$escolas[] = $eme['emeid'];
				}
			}
		}
		
		if($escolas) {
			
			$sql = "UPDATE emi.ementidade SET emestatus = 'I', emiexercicio = '{$_SESSION['exercicio']}' WHERE emeid IN('".implode("','",$escolas)."')";
			$this->db->executar( $sql );
			
		}
			
		if($dados['escolas'][0]) {
		
			foreach( $dados['escolas'] as $valor ) {
				
				$sql = "SELECT em.emeid, ende.estuf FROM emi.ementidade em
				 		LEFT JOIN entidade.endereco ende ON ende.entid = em.entid
						WHERE em.entid    = '".$valor."' AND
							  emiexercicio = '{$_SESSION['exercicio']}' AND 
							  em.tppid    = '".EMI_TIPO_ENTIDADE_ESCOLA."' AND 
							  em.emeidpai = '".$_SESSION["emi"]["emeidPai"]."'";
				
				$emeid = $this->db->pegaLinha($sql);
				
				if($emeid) {
					$sql = "UPDATE emi.ementidade SET emestatus = 'A', emiexercicio = '{$_SESSION['exercicio']}' WHERE emeid='".$emeid['emeid']."'";
										
				} else {
					$sql = "INSERT INTO emi.ementidade ( entid, tppid, emeidpai, emestatus, estuf , emiexercicio)
												VALUES ( {$valor}, " . EMI_TIPO_ENTIDADE_ESCOLA . ", {$_SESSION["emi"]["emeidPai"]}, 'A', ".(($emeid['estuf'])?"'".$emeid['estuf']."'":"NULL")." , '{$_SESSION['exercicio']}')";
				}
				
				$this->db->executar( $sql );
				
			}
		
		}
		
		
		$this->db->commit();
		$this->db->sucesso("principal/selecionaEscolas");
		
	}
	
	public function cadastraPapEscola( $dados ){
		
		$docid = emiPegarDocid( $dados['emeid'] );
		$boCorrecao = verificaSeCorrecao($docid);
		$papflagalterado = ($boCorrecao) ? 'true':'false';

		if( $dados["papid"] ){
			
			$dados = self::trataDados( $dados );
			
			$sql = "UPDATE 
						emi.empap
					SET
						papcaoatividade = {$dados["papcaoatividade"]},
						papmeta = {$dados["papmeta"]},
						papflagalterado = {$papflagalterado},
						papexercicio = '{$_SESSION['exercicio']}' 
					WHERE
						papid = {$dados["papid"]}";
			
		}else{
			
			$dados = self::trataDados( $dados );
			
			$sql = "INSERT INTO emi.empap( comid, 
									  emeid, 
									  papcaoatividade, 
									  papmeta, 
									  papflagalterado,
									  papstatus ,
									  papexercicio )
							  VALUES( '".$dados["comid"]."', 
									  '".$dados["emeid"]."', 
									  ".(($dados["papcaoatividade"])?$dados["papcaoatividade"]:"NULL").", 
									  ".(($dados["papmeta"])?$dados["papmeta"]:"NULL").", 
									  ".$papflagalterado.", 
									  'A' ,
									  '{$_SESSION['exercicio']}' )";
									  
		}
		
		$this->db->executar( $sql );
		
		$this->db->commit();
		
		print "	<script>
					window.parent.opener.location.href = window.opener.location;
				</script>";
		
		$this->db->sucesso( "principal/popupAcaoPap" );
		
	}
	
	public function cadastraGapEscola( $dados ){
		
		$docid = emiPegarDocid( $dados['emeid'] , "G" );
		$boCorrecao = verificaSeCorrecao($docid);
		$papflagalterado = ($boCorrecao) ? 'true':'false';
		
		if( $dados["papid"] ){
			
			$sql = "UPDATE 
						emi.emgap
					SET
						papcaoatividade = '{$dados["papcaoatividade"]}',
						papmeta = '{$dados["papmeta"]}',
						papflagalterado = {$papflagalterado},
						papexercicio = '{$_SESSION['exercicio']}' 
					WHERE
						papid = {$dados["papid"]}";
			
		}else{
			
			$sql = "INSERT INTO emi.emgap( mcpid, 
									  emeid, 
									  papcaoatividade, 
									  papmeta, 
									  papflagalterado,
									  papstatus ,
									  papexercicio )
							  VALUES( '".$dados["mcpid"]."', 
									  '".$dados["emeid"]."', 
									  ".(($dados["papcaoatividade"])?"'".$dados["papcaoatividade"]."'":"NULL").", 
									  ".(($dados["papmeta"])?"'".$dados["papmeta"]."'":"NULL").", 
									  ".$papflagalterado.", 
									  'A' ,
									  '{$_SESSION['exercicio']}' )";
									  
		}
		
		$this->db->executar( $sql );
		
		$this->db->commit();
		
		print "	<script>
					window.parent.opener.location.href = window.opener.location;
					window.close();
				</script>";
		
	}
	
	public function pegaDadosGap( $dados )
	{
		$sql = "select papcaoatividade, papmeta, papid from emi.emgap where papid = {$dados['papid']}";
		$arrDados = $this->db->pegaLinha($sql);
		$arrDados["papcaoatividade"] = iconv("ISO-8859-1", "UTF-8", trim($arrDados["papcaoatividade"]));
		$arrDados["papmeta"] 		 = iconv("ISO-8859-1", "UTF-8", trim($arrDados["papmeta"]));  
		echo simec_json_encode($arrDados);
		exit;
	}
	
	public function excluirGap( $dados )
	{
		$sql = "UPDATE emi.emgap SET papstatus = 'I' WHERE papid = {$dados['papid']}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		
		print "	<script>
					window.parent.opener.location.href = window.opener.location;
					window.close();
				</script>";
		exit;
	}
	
	public function salvaProfissionais( $dados )
	{
		
		$arrMacroCampos = pegaMacroCampos();
		
		foreach($arrMacroCampos as $mc)
		{
			$sql = "select preid from emi.profissionalenvolvido where mcpid = {$mc['mcpid']} and emeid = {$dados['emeid']}";
			$preid = $this->db->pegaUm($sql);
			
			$preqtdprofessor = $dados['num_prof_'.$mc['mcpid']] ? str_replace(".","",$dados['num_prof_'.$mc['mcpid']]) : "NULL";
			$preqtddirecao = $dados['num_equipe_'.$mc['mcpid']] ? str_replace(".","",$dados['num_equipe_'.$mc['mcpid']]) : "NULL";
			$preqtdoutros = $dados['num_outros_'.$mc['mcpid']] ? str_replace(".","",$dados['num_outros_'.$mc['mcpid']]) : "NULL";
			
			if($preid){
				$sql = "update
							emi.profissionalenvolvido
						set
							preqtdprofessor = $preqtdprofessor,
							preqtddirecao = $preqtddirecao,
							preqtdoutros = $preqtdoutros
						where
							preid = $preid;";
			}else{
				$sql = "insert into 
							emi.profissionalenvolvido
						(mcpid,preqtddirecao,preqtdoutros,preqtdprofessor,emeid)
							values
						({$mc['mcpid']},$preqtddirecao,$preqtdoutros,$preqtdprofessor,{$dados['emeid']})";
			}
			$this->db->executar($sql);
				
		}
		$this->db->commit();
		$this->db->sucesso( "principal/gapsEscola" );
		
	}
	
	public function pegaProfissionaisGAP( $mcpid ){
		
		$sql = "select * from emi.profissionalenvolvido WHERE mcpid = {$mcpid}";
		return $this->db->pegaLinha( $sql );		
	}
	
	public function excluirPap( $papid ){
		
		$sql = "UPDATE emi.empap SET papstatus = 'I' WHERE papid = {$papid}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		
		print "	<script>
					window.parent.opener.location.href = window.opener.location;
				</script>";
		
		$this->db->sucesso( "principal/popupAcaoPap" );
		
	}
	
	
	public function cadastraMatriz( $dados ){

		$dados = self::trataDados( $dados );
		
		$itfid			  = $dados["itfid"];
		$mdoespecificacao = substr($dados["mdoespecificacao"],0,1000);
		
		if( substr( $mdoespecificacao, -1 ) != "'" ){
			$mdoespecificacao = $mdoespecificacao . "'";	
		}
		
		$undid	 		  = $dados["undid"];
		$mdoqtd 		  = $dados["mdoqtd"];
		$mdovalorunitario = str_replace( ".", "",  $dados["mdovalorunitario"] );
		$mdovalorunitario = str_replace( ",", ".", $mdovalorunitario );
		$mdototal 		  = str_replace( ".", "",  $dados["mdototal"] );
		$mdototal 		  = str_replace( ",", ".", $mdototal );
		
		$docid = emiPegarDocid( $_SESSION['emi']['emeidPai'] );
		$boCorrecao = verificaSeCorrecao($docid);
		$papflagalterado = ($boCorrecao) ? 'true':'false';
		
		/*
		 * Alteração feita por Felipe Carvalho
		 * 25/11/2009
		 * Quando acorre inserção o '$dados["mdoid"]' está vindo como "''" e não está caindo na condição abaixo.
		 */
		if ( !$dados["mdoid"] || $dados["mdoid"] == "''" ){
			
			$sql = "INSERT INTO emi.emmatrizdistribuicaoorcamentar ( papid, unddid, itfid, mdoespecificacao, 
																	 mdoqtd, mdovalorunitario, mdototal, mdostatus, mdoflagalterado )
															VALUES ( {$dados["papid"]}, {$undid}, {$itfid}, {$mdoespecificacao},
																	 {$mdoqtd}, {$mdovalorunitario}, {$mdototal}, 'A', {$papflagalterado} )";			
																	 
		}else{
			
			$sql = "UPDATE
						emi.emmatrizdistribuicaoorcamentar
					SET
						itfid = {$itfid}, 
						unddid = {$undid}, 
						mdoespecificacao = {$mdoespecificacao}, 
						mdoqtd = {$mdoqtd}, 
						mdovalorunitario = {$mdovalorunitario}, 
						mdototal = {$mdototal},
						mdoflagalterado = {$papflagalterado}
					WHERE
						mdoid = {$dados["mdoid"]}";
			
		}

		$this->db->executar( $sql );
			
		$this->db->commit();
		$this->db->sucesso( "principal/matriz" );
	
	}
	
	public function cadastraMatrizGap( $dados ){

		$dados = self::trataDados( $dados );
		
		$itfid			  = $dados["itfid"];
		$mdoespecificacao = substr($dados["mdoespecificacao"],0,1000);
		
		if( substr( $mdoespecificacao, -1 ) != "'" ){
			$mdoespecificacao = $mdoespecificacao . "'";	
		}
		
		$undid	 		  = $dados["undid"];
		$mdoqtd 		  = $dados["mdoqtd"];
		$mdovalorunitario = str_replace( ".", "",  $dados["mdovalorunitario"] );
		$mdovalorunitario = str_replace( ",", ".", $mdovalorunitario );
		$mdototal 		  = str_replace( ".", "",  $dados["mdototal"] );
		$mdototal 		  = str_replace( ",", ".", $mdototal );
		
		$docid = emiPegarDocid( $_SESSION['emi']['emeidPai'] , "N");
		$boCorrecao = verificaSeCorrecao($docid);
		$papflagalterado = ($boCorrecao) ? 'true':'false';

		/*
		 * Alteração feita por Felipe Carvalho
		 * 25/11/2009
		 * Quando acorre inserção o '$dados["mdoid"]' está vindo como "''" e não está caindo na condição abaixo.
		 */
		if ( !$dados["mdoid"] || $dados["mdoid"] == "''" ){
			
			$sql = "INSERT INTO emi.emmatrizdistribuicaoorcamentargap ( papid, unddid, itfid, mdoespecificacao, 
																	 mdoqtd, mdovalorunitario, mdototal, mdostatus, mdoflagalterado )
															VALUES ( {$dados["papid"]}, {$undid}, {$itfid}, {$mdoespecificacao},
																	 {$mdoqtd}, {$mdovalorunitario}, {$mdototal}, 'A', {$papflagalterado} )";			
																	 
		}else{
			
			$sql = "UPDATE
						emi.emmatrizdistribuicaoorcamentargap
					SET
						itfid = {$itfid}, 
						unddid = {$undid}, 
						mdoespecificacao = {$mdoespecificacao}, 
						mdoqtd = {$mdoqtd}, 
						mdovalorunitario = {$mdovalorunitario}, 
						mdototal = {$mdototal},
						mdoflagalterado = {$papflagalterado}
					WHERE
						mdoid = {$dados["mdoid"]}";
			
		}

		$this->db->executar( $sql );
			
		$this->db->commit();
		$this->db->sucesso( "principal/matrizGap" );
	
	}

	public function buscaDadosItem( $mdoid ){
		
		$sql = "SELECT
					mdoid,
					itfid, 
					unddid,
					trim(mdoespecificacao) as mdoespecificacao, 
					mdoqtd, 
					mdovalorunitario, 
					mdototal
				FROM
					emi.emmatrizdistribuicaoorcamentar
				WHERE
					mdoid = {$mdoid}";
		
		$dados = $this->db->pegaLinha( $sql );
		
		$dados["mdoespecificacao"] = iconv("ISO-8859-1", "UTF-8", $dados["mdoespecificacao"]);
		$dados["mdovalorunitario"] = number_format( $dados["mdovalorunitario"], 2, ",", "." );
		$dados["mdototal"]		   = number_format( $dados["mdototal"], 2, ",", "." );
		
		echo simec_json_encode($dados);
		
	}
	
	public function buscaDadosItemGap( $mdoid ){
		
		$sql = "SELECT
					mdoid,
					itfid, 
					unddid,
					trim(mdoespecificacao) as mdoespecificacao, 
					mdoqtd, 
					mdovalorunitario, 
					mdototal
				FROM
					emi.emmatrizdistribuicaoorcamentargap
				WHERE
					mdoid = {$mdoid}";
		
		$dados = $this->db->pegaLinha( $sql );
		
		$dados["mdoespecificacao"] = iconv("ISO-8859-1", "UTF-8", $dados["mdoespecificacao"]);
		$dados["mdovalorunitario"] = number_format( $dados["mdovalorunitario"], 2, ",", "." );
		$dados["mdototal"]		   = number_format( $dados["mdototal"], 2, ",", "." );
		
		echo simec_json_encode($dados);
		
	}
	
	public function excluirItem( $mdoid ){
		
		$sql = "UPDATE emi.emmatrizdistribuicaoorcamentar SET mdostatus = 'I' WHERE mdoid = {$mdoid}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		$this->db->sucesso( "principal/matriz" );
		
	}
	
	public function excluirItemGap( $mdoid ){
		
		$sql = "UPDATE emi.emmatrizdistribuicaoorcamentargap SET mdostatus = 'I' WHERE mdoid = {$mdoid}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		$this->db->sucesso( "principal/matrizGap" );
		
	}
	
	public function salvaBeneficiarios( $dados ){
		
		$sql = "select benid from emi.emgap where papid = {$dados['papid']};";
		$benid = $this->db->pegaUm($sql);
		
		$benqtd1anovesp = $dados['vesp_1'] ? str_replace(".","",$dados['vesp_1']) : "null";
		$benqtd1anomat = $dados['mat_1'] ? str_replace(".","",$dados['mat_1']) : "null";
		$benqtd1anonot = $dados['not_1'] ? str_replace(".","",$dados['not_1']) : "null";
		$benqtd2anovesp = $dados['vesp_2'] ? str_replace(".","",$dados['vesp_2']) : "null";
		$benqtd2anomat = $dados['mat_2'] ? str_replace(".","",$dados['mat_2']) : "null";
		$benqtd2anonot = $dados['not_2'] ? str_replace(".","",$dados['not_2']) : "null";
		$benqtd3anovesp = $dados['vesp_3'] ? str_replace(".","",$dados['vesp_3']) : "null";
		$benqtd3anomat = $dados['mat_3'] ? str_replace(".","",$dados['mat_3']) : "null";
		$benqtd3anonot = $dados['not_3'] ? str_replace(".","",$dados['not_3']) : "null";
		
		if($benid){
			$sql = "update
						emi.beneficiario
					set
						benqtd1anovesp = $benqtd1anovesp,
						benqtd1anomat = $benqtd1anomat,
						benqtd1anonot = $benqtd1anonot,
						benqtd2anovesp = $benqtd2anovesp,
						benqtd2anomat = $benqtd2anomat,
						benqtd2anonot = $benqtd2anonot,
						benqtd3anovesp = $benqtd3anovesp,
						benqtd3anomat = $benqtd3anomat,
						benqtd3anonot = $benqtd3anonot
					where
						benid = $benid";
			$this->db->executar( $sql );
		}else{
			$sql = "insert into 
						emi.beneficiario 
					(benqtd1anovesp,benqtd1anomat,benqtd1anonot,benqtd2anonot,benqtd2anovesp,benqtd2anomat,benqtd3anovesp,benqtd3anomat,benqtd3anonot) 
						values 
					($benqtd1anovesp,$benqtd1anomat,$benqtd1anonot,$benqtd2anonot,$benqtd2anovesp,$benqtd2anomat,$benqtd3anovesp,$benqtd3anomat,$benqtd3anonot)
						returning benid;";
			$benid = $this->db->pegaUm($sql);
			$sql = "update
						emi.emgap
					set
						benid = $benid
					where
						papid = {$dados['papid']}";
			$this->db->executar( $sql );
		}
		
		$this->db->commit();
		$this->db->sucesso( "principal/matrizGap" );
		
	}
	
	public function pegaBeneficiarios($papid)
	{
		$sql = "select * from emi.beneficiario where benid = (select benid from emi.emgap where papid = $papid)";
		return $this->db->pegaLinha($sql);
	}
	
	public function getDocid($emeid)
	{
		$sql = "select docid from emi.ementidade where emeid = $emeid";
		return $this->db->pegaUm($sql);
	}
	
}

?>