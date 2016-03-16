<?php

class ies{
	
	public function __construct(){
		
		global $db;
		$this->db  = $db;
		
	}
	
	public function trataDados( $dados = array() ){
		
		if ( is_array( $dados ) ){
			foreach ( $dados as $campo=>$valor ){
				if ( !is_numeric( $valor ) ){
					$dados[$campo] = !empty( $valor ) ? "'" . pg_escape_string( trim( $valor ) ) . "'" : "''";
				}else{
					$dados[$campo] = !empty( $valor ) ? $valor : "NULL";
				}
			}
		}else{
			if ( !is_numeric( $dados ) ){
				$dados = !empty( $dados ) ? "'" . pg_escape_string( trim( $dados ) ) . "'" : "''";
			}else{
				$dados = !empty( $dados ) ? $dados : "NULL";
			}
		}
		
		return $dados;
		
	}
	
	public function trataString( $string ){
		
		$string = str_replace( "-", "", $string );
		$string = str_replace( ".", "", $string );
		
		return $string;
		
	}
	
	public function cabecalhoIES( $iesid ){
		
		$sql = "SELECT 
					iesnome as nome,
					CASE WHEN ie.iesmunicipio <> '' AND ie.iesuf <> '' 
					THEN ie.iesmunicipio || ' / ' || ie.iesuf 
					ELSE 'Não Informado' END as endereco
				FROM 
					ies.ies ie
				WHERE 
					ie.iesid = {$iesid}";
		
		$dados = $this->db->pegaLinha( $sql );
		
		print "<table class='tabela' width='95%' bgcolor='#f5f5f5' cellspacing='1' cellpadding='2' align='center'>"
			. "    <tr>"
			. "        <td class='subtitulocentro' colspan='2'>Dados da Instituição</td>"
			. "    </tr>"
			. "    <tr>"
			. "        <td class='subtitulodireita' width='190px'>Instituição de Ensino Superior</td>"
			. "	       <td>{$dados["nome"]}</td>"
			. "    </tr>"
			. "    <tr>"
			. "        <td class='subtitulodireita'>Município / UF</td>"
			. "	       <td>{$dados["endereco"]}</td>"
			. "    </tr>"
			. "</table>";
		
	}
	
	public function montaInicioCadastrador( $iesid ){
		
		if( $iesid ){
		
			$pbiid = iesPegaProjeto( $iesid );
			
			if ( iesVerificaAnexo( $pbiid ) ){
				
				print "<script>location.href='ies.php?modulo=principal/projeto&acao=A';</script>";
				die;
				
			}else{
			
				print "<table class='tabela' width='95%' bgcolor='#f5f5f5' cellspacing='1' cellpadding='2' align='center'>"
					. "    <tr>"
					. "        <td align='center'>"
					. "			   <p align='justify'>
										<br/> O <b>Programa IES - MEC/BNDES</b> é resultado de uma atuação conjunta entre o Ministério da Educação e o Banco Nacional de Desenvolvimento Econômico e Social e tem por finalidade oferecer recursos financeiros, na forma de financiamento, às instituições de educação superior, públicas ou privadas, com ou sem fins lucrativos, inclusive beneficentes de assistência social. Os financiamentos serão concedidos pelo BNDES, com o intermédio de Instituições Financeiras Credenciadas - IFC, a projetos que visem a melhoria da qualidade da educação superior, compreendendo atividades de ensino, pesquisa, extensão e gestão acadêmica. É imprescindível que os projetos demonstrem articulação entre os itens cujo financiamento foi solicitado e a elevação nos níveis de qualidade da instituição proponente. <br/><br/>" 
					. "				</p>"
					. "		   </td>"
					. "    </tr>"
					. "    <tr bgcolor='#D0D0D0'>"
					. "	       <td> <input type='button' value='Protocolar Projeto' onclick='iesPreencherIntencao();' style='cursor:pointer;'> </td>"
					. "    </tr>"
					. "</table>";
				
			}
			
		}else{
			
			print "<table class='tabela' width='95%' bgcolor='#f5f5f5' cellspacing='1' cellpadding='2' align='center'>"
				. "    <tr>"
				. "        <td align='center' style='color:red;'>Seu perfil não possui uma Instituição associada. <br/> Entre em contato com o Administrador do sistema para maiores informações.</td>"
				. "    </tr>"
				. "</table>";
		}
		
	}
	
	public function buscaDadosInstituicao( $iesid ){
		
		$sql = "SELECT 
					'(' || iescnpjmantenedora || ') ' || iesnomemantenedora || '(' || iescodigomantenedora || ')' as mantenedora,
					CASE WHEN iesnaturezajuridica <> '' THEN iesnaturezajuridica ELSE '-' END as naturezamantenedora,
					ie.iesnome || ' (' || ie.iescodigo || ')' as nome,
					CASE WHEN iesendereco <> '' THEN iesendereco ELSE '-' END as endereco,
					CASE WHEN iesnumero <> '' THEN iesnumero ELSE '-' END as numero,
					CASE WHEN iescomplemento <> '' THEN iescomplemento ELSE '-' END as complemento,
					CASE WHEN iescep <> '' THEN iescep ELSE '-' END as cep,
					CASE WHEN iesbairro <> '' THEN iesbairro ELSE '-' END as bairro,
					ie.iesmunicipio|| ' / ' || ie.iesuf as municipio,
					CASE WHEN iestelefone  <> '' THEN ie.iestelefone  ELSE '-' END as tel,
					CASE WHEN iesfax <> '' THEN iesfax ELSE '-' END as fax,
					CASE WHEN iesorganizacaoacad <> '' THEN iesorganizacaoacad ELSE '-' END as organizacao,
					CASE WHEN iesresplegal <> '' THEN ie.iesresplegal ELSE '-' END as responsavel,
					CASE WHEN iesemail <> '' THEN iesemail ELSE '-' END as email,
					CASE WHEN iessitio <> '' THEN '<a href=\"http://' || iessitio || '\" target=\"_blank\">' || iessitio || '</a>' ELSE '-' END as sitio,
					CASE WHEN iesead = 'S' THEN '<img src=\"/imagens/inclui_p.gif\">' ELSE '<img src=\"/imagens/exclui_p.gif\">' END as distancia
 				FROM 
					ies.ies ie 
				WHERE 
					iesid = {$iesid}";
		
		return $this->db->pegaLinha( $sql );
		
	}
	
	public function criaProjetoIes( $iesid ){
		
		if ( !iesPegaProjeto($iesid) ){
			
			$sql = "INSERT INTO ies.projetobndesies (iesid, pbistatus) 
					VALUES ( {$iesid}, 'A' )";
			
			$this->db->pegaUm( $sql );
			
			$this->db->commit();
				
		}
		
	}
	
	public function montaInicioValidacao(){
		
		$sql = "SELECT
					'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || wd.esdid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || wd.esdid || ' );abreconteudo(\'ies.php?modulo=inicio&acao=C&subAcao=gravarCarga&carga=' || wd.esdid || '&params=\' + params, ' || wd.esdid || ');\"/></center>' as acao,
					we.esddsc as descricao,
					count(pbiid) as total,
					'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || wd.esdid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM
					workflow.documento wd
				LEFT JOIN
					workflow.estadodocumento we ON we.esdid = wd.esdid
				LEFT JOIN
					ies.projetobndesies pb ON pb.docid = wd.docid
				WHERE 
					wd.tpdid = " . IES_TIPO_DOCUMENTO . " AND pbistatus = 'A'
				GROUP BY
					descricao, wd.esdid";
		
		print "<table class='tabela' width='95%' bgcolor='#f5f5f5' cellspacing='1' cellpadding='2' align='center'>"
			. "    <tr>"
			. "        <td class='subtitulocentro'>Status</td>"
			. "    </tr>"
			. "</table>";

		$cabecalho = array( "Ação", "Estado Atual", "Quantidade de IES" );
			
		$this->db->monta_lista( $sql, $cabecalho, 20, 4, 'N','center', '', '', '', '' );
		
	}
	
	public function filtraListaInstituicao( $dados ){
		
		$filtro .= !empty( $dados["iesid"] ) 	 ? " AND ie.iesid  = {$dados["iesid"]}" 	  : "";
		$filtro .= !empty( $dados["estuf"] ) 	 ? " AND ie.iesuf  = '{$dados["estuf"]}'" 	  : "";
		$filtro .= !empty( $dados["pbistatus"] ) ? " AND pbistatus = '{$dados["pbistatus"]}'" : "";
		
		return $filtro;
		
	}
	
	public function montaListaInstituicoes( $esdid, $filtro ){
		
		$btExcluir = ( !iesPossuiPerfil( IES_CONSULTAGERAL ) && !iesPossuiPerfil( IES_COMISSAOAVALIADORA ) ) ? "<img src=\"/imagens/exclui_p.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"cancelaIES(' || ie.iesid || ');\"/>" : "<img src=\"/imagens/exclui_p2.gif\" border=0 title=\"Excluir\"/>";
		
		$sql = "SELECT DISTINCT
					CASE WHEN pbistatus = 'A' 
						THEN '<center>
								<img src=\"/imagens/check_p.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"abreDadosIES(' || ie.iesid || ');\"/>
								{$btExcluir}
							  </center>' 
						ELSE '<center>
								<img src=\"/imagens/check_p.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"abreDadosIES(' || ie.iesid || ');\"/>
								{$btExcluir}
							  </center>'
						END as acao,
					iescodigo as codigo,
					'<a style=\"cursor:pointer;\" onclick=\"abreDadosIES(' || ie.iesid || ');\">' || iesnome || '</a>' as descricao,
					ie.iesmunicipio || ' / ' || ie.iesuf as endereco,
					pbivalorprojeto as valor,
					to_char(htddata, 'DD/MM/YYYY') as dtinicio,
					(to_char(htddata, 'DD')::integer - to_char(current_date, 'DD')::integer)as qtddias,
					CASE WHEN ap.pbiid is not null 
						 THEN '<center><img src=\"/imagens/anexo.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\"/></center>'
						 ELSE '<center> - </center> ' END as anexo
				FROM
					ies.ies ie
				INNER JOIN
					ies.projetobndesies pb ON pb.iesid = ie.iesid
				LEFT JOIN
					( SELECT max(aprid), pbiid FROM ies.arquivosprojeto GROUP BY pbiid) ap ON ap.pbiid = pb.pbiid
				LEFT JOIN 
					workflow.documento wd ON wd.docid = pb.docid 
				LEFT JOIN
					workflow.historicodocumento wh ON wh.hstid = wd.hstid
				--	(SELECT max(hstid), docid, htddata FROM workflow.historicodocumento GROUP BY docid, htddata) wh ON wh.docid = pb.docid
				WHERE 
					esdid = {$esdid}{$filtro} AND pbistatus = 'A'
				ORDER BY
					descricao, acao, codigo, endereco, valor, dtinicio, qtddias, anexo";
		
		$cabecalho = array( "Ação", "Código", "Instituição de Ensino", "Município/UF", "Valor", "Data de Início", "Qtd. de Dias", "Anexos" );
		
		$this->db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
		
	}

	public function cancelaProjetoInstituicao( $iesid ){
		
		$sql = "UPDATE ies.projetobndesies SET pbistatus = 'I' WHERE iesid = {$iesid}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		$this->db->sucesso( "principal/listaInstituicoes" );
	}
	
	public function cadastraResponsavelIes( $entid ){
		
		$sql = "UPDATE ies.projetobndesies SET entidresponsavel = {$entid} WHERE iesid = {$_SESSION["ies"]["iesid"]}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		$this->db->sucesso( "principal/dadosResponsavel" );
		
	}
	
	public function montaListaProjetos( $pbiid, $acao ){
		
		$campo = $acao == 'A' && (iesPegarEstadoAtual( $pbiid ) != AGUARDANDO_VALIDACAO_CRITERIOS ) ? "<img src=\"/imagens/exclui_p.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"iesExcluiAnexo(' || aprid || ');\"/>" : 
								"<img src=\"/imagens/exclui_p2.gif\" border=0/>" ;
		
		$sql = "SELECT
					'<center>
						{$campo}
					</center>' as acao,
					to_char(arqdata, 'DD/MM/YYYY'),
					'<a style=\"cursor: pointer; color: blue;\" onclick=\"iesDownloadArquivo( ' || ia.arqid || ', \'{$acao}\' );\" />' || pa.arqnome || '.'|| pa.arqextensao ||'</a>',
					CASE WHEN aprtipo = 'C' THEN 'Carta Intenção' ELSE 'Projeto' END as tipo,
					usu.usunome
				FROM
					ies.arquivosprojeto ia
				INNER JOIN
					public.arquivo pa ON pa.arqid = ia.arqid
				INNER JOIN
					seguranca.usuario usu ON usu.usucpf = pa.usucpf
				WHERE
					pbiid = {$pbiid} AND sisid = 66";
		
		$cabecalho = array( "Ação", "Data de Inclusão", "Nome do Arquivo", "Tipo", "Inserido Por" );
		
		$this->db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
		
	}

	public function cadastraProjetoIes( $dados, $arquivos ){
		
		$projeto = $arquivos["projeto"];
		
		if( $projeto["type"] == "application/exe"   || $projeto["type"] == "application/bat" || $projeto["type"] != "application/pdf" ){
            
			print "<script>alert('Não é possível enviar este tipo de arquivo!');</script>";
			return false;
			
		}
				
		/** Projeto **/
		
		$arqid   = "";
		$caminho = "";
		
		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo (arqnome,arqextensao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
				VALUES('".current(explode(".", $projeto["name"]))."','".end(explode(".", $projeto["name"]))."','".$projeto["type"]."','".$projeto["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
		$arqid = $this->db->pegaUm($sql);

		//Insere o registro na tabela ies.arquivosprojeto
		$sql = "INSERT INTO ies.arquivosprojeto ( pbiid, arqid, aprtipo )
				VALUES( {$dados["pbiid"]}, {$arqid}, 'P' );";
		$this->db->executar($sql);
		
		$caminho = '../../arquivos/ies/' . floor($arqid/1000) . '/';
		
		if( !is_dir($caminho) ) {
			mkdir($caminho, 0777, true);
		}
		
		move_uploaded_file( $projeto["tmp_name"], $caminho.$arqid );
		
		$this->db->commit();
		$this->db->sucesso("principal/projeto", "");
		
		
	}
	
	public function downloadArquivo( $arqid ){
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = {$arqid}";
		$arquivo = current($this->db->carregar($sql));
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
		if ( !is_file( $caminho ) ) {
			$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
		}
		$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
		header( 'Content-type: '. $arquivo['arqtipo'] );
		header( 'Content-Disposition: attachment; filename='.$filename);
		readfile( $caminho );
		exit();
		
	}
	
	public function excluirArquivo( $aprid ){
		
		$sql   = "SELECT arqid FROM ies.arquivosprojeto WHERE aprid = {$aprid}";
		$arqid = $this->db->pegaUm( $sql );
		
		$sql = "DELETE FROM ies.arquivosprojeto WHERE aprid = {$aprid}";
		$this->db->executar( $sql );
		
		$sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = {$arqid}";
		$this->db->executar( $sql );
		
		$caminho = '../../arquivos/ies/' . floor($arqid/1000) . '/' . $arqid;
		unlink( $caminho );
		
		$this->db->commit();
		$this->db->sucesso("principal/projeto", "");
		
	}
	
}

?>