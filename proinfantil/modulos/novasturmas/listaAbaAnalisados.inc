<td>
	
	<?php 
		
		$arrAbas =	array(
	 		0 => array( "descricao" => "Aguardando Análise",	"link"	  => "proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A"),
			1 => array( "descricao" => "Analisados",	 		"link"	  => "proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A&abas=listaAbaAnalisados")
		);
		
		
		echo montarAbasArray( $arrAbas, "proinfantil.php?modulo=novasturmas/analiseTurmas&acao=A&abas=listaAbaAnalisados" );
			
	?>
	
	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" style="width: 100%"  cellspacing="1" cellpadding="3">
		<tr>
			<th colspan="2">Lista de Turmas Enviadas para Análise</th>
		</tr>
		
		<tr>
			<td colspan="2" >
				<?php
				$boHabilita = false;
				if( in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( PERFIL_ANALISTA, $perfis )  ){
					$boHabilita = true;
				}
				
				$arrTurmas = $obNovasTurmas->carregaTurmaPorMunicipio('abaAnalisados');
				
				$arrRegistro = array();
				$valor_total = 0;
				$boMostraParecer = false;
				
				foreach ($arrTurmas as $key => $v) {
					
					if( $v['anatipo'] == 'A' ){
						$valortotal = str_replace(".","", $v['valor_total']);
						$valortotal = str_replace(",",".", $valortotal);
						
						$valor_total += $valortotal;
					}
					
					if( $boHabilita ){
						$acao = "
							<center><img id=\"imgmais".$v['turid']."\" name=\"imgmais".$v['turid']."\" src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" onclick=\"abrirTurma(".$v['turid'].")\"/>
							<img id=\"imgmenos".$v['turid']."\" name=\"imgmenos".$v['turid']."\" src=\"../imagens/menos.gif\" style=\"cursor:pointer;display: none;\" onclick=\"fecharTurma(".$v['turid'].")\"/>
							&nbsp;<img src=\"../imagens/consultar.gif\" id=\"".$v['turid']."\" title=\"Dados da Turma\" onclick=\"abrirTurmaPopup('".$v['turid']."', '".$v['muncod']."')\" style=\"cursor:pointer;\"/>
							<img src=\"../imagens/editar_conteudo_caixa.png\" id=\"".$v['turid']."\" title=\"Parecer/Análise\" onclick=\"inserirAnaliseTurma(".$v['turid'].", '')\" style=\"cursor:pointer;\"/>";	
					} else {
						$acao = "
							<center><img id=\"imgmais".$v['turid']."\" name=\"imgmais".$v['turid']."\" src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" onclick=\"abrirTurma(".$v['turid'].")\"/>
							<img id=\"imgmenos".$v['turid']."\" name=\"imgmenos".$v['turid']."\" src=\"../imagens/menos.gif\" style=\"cursor:pointer;display: none;\" onclick=\"fecharTurma(".$v['turid'].")\"/>
							&nbsp;<img src=\"../imagens/consultar.gif\" id=\"".$v['turid']."\" title=\"Dados da Turma\" onclick=\"abrirTurmaPopup('".$v['turid']."', '".$v['muncod']."')\" style=\"cursor:pointer;\"/>";						
					}
					$arPeriodo = explode('/',$v['periodo']) ;
					
					$acaoParecer = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</center>";
					if( !empty($v['anaid']) ){
						$acaoParecer = "&nbsp;<img src=\"../imagens/report.gif\" id=\"".$v['turid']."\" title=\"Histórico Parecer\" onclick=\"abrirParecerTurma(".$v['turid'].", '".$v['muncod']."')\" style=\"cursor:pointer;\"/></center>";
					}
					
					$arrRegistro[$key] = array(
											'acao' 				=> $acao.$acaoParecer,
											'turdsc' 			=> $v['turdsc'],
											'lote' 				=> $v['lote'],
											'tirdescricao' 		=> $v['tirdescricao'],
											'tipoatendimento' 	=> $v['tipoatendimento'],
											'entcodent' 		=> $v['entcodent'],
											'turdtinicio' 		=> $v['turdtinicio'],
											'dtanalise' 		=> $v['dtanalise'],
											'periodo' 			=> '&nbsp;'.$arPeriodo[0],
											'tipoanalise' 		=> $v['tipoanalise'],
											'esddsc' 			=> $v['esddsc'],
											);
											
					if( $v['anatipo'] == 'A' && $v['esdid'] == WF_NOVASTURMAS_EM_VALIDACAO_DE_ANALISE ){
						$boMostraParecer = true;
					}
				}
				
				$arrCab = array("Ação","Nome da Turma", "Lote", "Tipo de Rede","Atendimento","Código INEP","Data Início da Turma", "Data Envio Análise", "Qtd de meses para Repasse dos recursos", "Tipo Análise", 'Situação Workflow');
				$db->monta_lista_simples($arrRegistro, $arrCab, 100000, 10, "N", "100%", "N", true, false, false, true); ?>	
			</td>
		</tr>