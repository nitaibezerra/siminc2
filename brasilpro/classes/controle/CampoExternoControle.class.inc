<?php

class CampoExternoControle{

	private $htm;
	private static $camposDefault;

	//Função obrigatória. Nessa função deverá ser montado a questão que aparecerá dentro do campo externo. Deverá ser colocada dentro do $this->htm.
	public function montaNovoCampo( $perid, $qrpid, $percent = 90 ){

		$obQestionario = new QQuestionarioResposta();

		$queid = $obQestionario->pegaQuestionario( $qrpid );

		if( $queid == QUESTOES_PONTUAIS_QUEID ){ //questões pontuais
			global $db;

			$tp = $perid == QUESTOES_PONTUAIS_ESCOLAS ? 1 : 2;

			$sql = "SELECT count(perid) FROM questionario.resposta WHERE perid = {$perid} AND qrpid = {$qrpid}";
			$qnt = $db->pegaUm( $sql );
			if( $qnt == 0 ){
				$obQuest = new QuestoesPontuaisAnexos();
				$dR = $obQuest->pegaResposta( $qrpid, $perid );
				if( $dR ){
					$obQuest->deletaAnexo( $dR['arqid'] );
				}
			}

			$obQuest = new QuestoesPontuaisAnexos( );
			$dadosResp = $obQuest->pegaResposta( $qrpid, $perid );

			$obQuest = new QuestoesPontuaisEscolas( );
			$quantEsc = $obQuest->quantidadeEscola( $qrpid, $perid );


			if ( !self::$camposDefault ){
	//			$htm .= "<input type='hidden' name='ajax' id='ajax' value='1'>";
				$htm .= "<input type='hidden' name='salvar_questionario' id='salvar_questionario' value='1'>";
				$htm .= "<input type='hidden' name='perid' id='perid' value=''>";
				$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";
				$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";

				self::$camposDefault = true;
			}
			$htm .= "<input type='hidden' name='peridExterno[]' id='peridExterno_{$perid}' value='{$perid}'>";
			$htm .= "<input type='hidden' name='arqid[{$perid}]' id='arqid' value='1'>";
			$htm .= "<table class='tabela' style='width:{$percent}%;' align='center'>";
			$htm .= "<tr>";
			$htm .= "<td>";
			if( $tp == 2 ){
				if( $dadosResp['arqid'] ){
					$htm .= "<a href=\"#\" onclick=\"excluirArquivo( ".$dadosResp['arqid'].", ".$perid." );\" title=\"Excluir Anexo\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a> Arquivo em anexo: <a onclick=\"janela('?modulo=principal/questoesPontuais&acao=A&requisicao=download&arqid={$dadosResp['arqid']}', 600, 480);\">".$dadosResp['arqdescricao']."</a>";
				} else {
					$htm .= "<input type='file' name='arquivo_{$perid}' />";
				}
			} else {
				$htm .= "<font style='font-family: Arial; color: black; font-size: 12px;'><b>Selecione as escolas com CE implantado. <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=0 title=\"Alterar Curso\" onclick=\"janela('?modulo=principal/popupSelecionaEstados&acao=A&qrpid={$qrpid}', 600, 480);\"></b></font><br>";
				$htm .= "<font style='font-family: Arial; color: black; font-size: 14px;'>Total de escolas da rede municipal de ensino: ".campo_texto( 'totalEscola', 'N', 'N', '', 6, 4, '####', '', '', '', '', 'id="totalEscola"' ,'' , $quantEsc )."</font><br>";
			}
			$htm .= "</td>";
			$htm .= "</tr>";
	  		$htm .= "</table>";

	  		$this->htm = $htm;

		} elseif( $queid == QUESTIONARIOPROMUN || $queid == QUESTIONARIOPROEST ) {
			global $db;

			$bgcolor = "#A8A8A8";
			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
			$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";

			$arrPerid = array( 1735, 1736, 1737, 1738, 1739, 1740, 1741, 1742, 1571,1572,1573,1574,1576,1577,1578,1579 );

			if( in_array( $perid, $arrPerid ) ){

				$sql = "SELECT COALESCE(sum(qpmqtd),0) FROM par.questionarioprofuncionariomun WHERE qrpid = " . $qrpid ." AND perid = " . $perid;
				$qtdMun = $db->pegaUm( $sql );

				$htm .= "<input type='hidden' name='proest' id='proest' value='1'>";
		    	$htm .= "<table style='width:100%;' align='center'>";
				$htm .= "<tr>";
		    	$htm .= "<td><input type=\"text\" title=\"quantidade\" name=\"quant\" class=\"CampoEstilo\" id=\"quant\" value=\"".$qtdMun."\" onclick=\"return popupSelecionarMunicipios( ".$perid.", ".$qrpid." );\" maxlength=\"3\" /><img src=\"../imagens/obrig.gif\" /></td>";
				$htm .= "</tr>";
		    	$htm .= "</table>";

			} else {
				$sql = "SELECT * FROM par.questionarioprofuncionariotutor WHERE qrpid = ".$qrpid;
				$arrDados = $db->carregar( $sql );

				$htm .= "<input type='hidden' name='promun' id='promun' value='1'>";
				$htm .= "<input type='hidden' name='quantidadeBanco' id='quantidadeBanco' value='{$arrDados[0]['qptqnt']}'>";

		    	$htm .= "<table style='width:100%;' align='center'>";
				$htm .= "<tr>";
		    	$htm .= "<td><input type=\"text\" title=\"quantidade\" name=\"quant\" class=\"CampoEstilo\" id=\"quant\" value=\"".$arrDados[0]['qptqnt']."\" onblur=\"geraTabela( this.value );\" maxlength=\"3\" />&nbsp;&nbsp;<input type=\"button\" value=\"Criar\"><img src=\"../imagens/obrig.gif\" /></td>";
				$htm .= "</tr>";
				$htm .= "<tr>";
				$htm .= "<td>";

				if( $arrDados[0]['qptqnt'] ){
					$htm .= "<table class='tabela' id='tabpro' style='width:100%; display:block;' align='center'>";

					$htm .= "<tr>";
					$htm .= "<td bgcolor='#A8A8A8' align='center'><b>CPF</b></td>";
					$htm .= "<td bgcolor='#A8A8A8' align='center' width='25%'><b>Nome</b></td>";
					$htm .= "<td bgcolor='#A8A8A8' align='center'><b>E-mail</b></td>";
					$htm .= "<td bgcolor='#A8A8A8' align='center'><b>Ano de Formação</b></td>";

					if($queid == QUESTIONARIOPROMUN){
						$txtQueid = 'municipal';
					} else {
						$txtQueid = 'estadual';
					}
					$htm .= "<td bgcolor='#A8A8A8' align='center'><b>Em exercício na rede<br>$txtQueid de ensino?</b></td>";
					$htm .= "</tr>";
					$i = 0;
					foreach( $arrDados as $valor ){
						$htm .= "<tr>";
				    	$htm .= "<td><input type=\"text\" name=\"cpf[]\" title=\"CPF\" onkeyup=\"this.value=mascaraglobal('###.###.###-##', this.value);\" onblur=\"getEnderecoPeloCEP(this.value,'1'); \" class=\"CampoEstilo\" id=\"cpf_{$i}\" value=\"".$valor['qptcpf']."\" style=\"width: 100%\" maxlength=\"14\" /></td>";
				    	$htm .= "<td><input type=\"hidden\" name=\"nome[]\" title=\"NOME\" id=\"nome_{$i}\" value=\"".$valor['qptnome']."\" style=\"width: 100%\" maxlength=\"50\" /><label>".$valor['qptnome']."</label></td>";
				    	$htm .= "<td><input type=\"text\" name=\"email[]\" title=\"EMAIL\" id=\"email_{$i}\" value=\"".$valor['qptemail']."\" style=\"width: 100%\" maxlength=\"30\" /></td>";
				    	$htm .= "<td><select name=\"ano[]\" id=\"ano_{$i}\" style=\"width: 100%\">
				    					<option value=\"0\">Selecione...</option>
				    					<option ".( $valor['qptano'] == 2005 ? "selected" : ""  )." value=\"2005\">2005</option>
				    					<option ".( $valor['qptano'] == 2006 ? "selected" : ""  )." value=\"2006\">2006</option>
				    					<option ".( $valor['qptano'] == 2007 ? "selected" : ""  )." value=\"2007\">2007</option>
				    					<option ".( $valor['qptano'] == 2008 ? "selected" : ""  )." value=\"2008\">2008</option>
				    					<option ".( $valor['qptano'] == 2009 ? "selected" : ""  )." value=\"2009\">2009</option>
				    					<option ".( $valor['qptano'] == 2010 ? "selected" : ""  )." value=\"2010\">2010</option>
				    			</select></td>";
				    	$htm .= "<td><select name=\"emexercicio[]\" id=\"emexercicio_{$i}\" style=\"width: 100%\">
				    					<option value=\"0\">Selecione...</option>
				    					<option ".( $valor['qptexerc'] == 's' ? "selected" : ""  )." value=\"s\">Sim</option>
				    					<option ".( $valor['qptexerc'] == 'n' ? "selected" : ""  )." value=\"n\">Não</option>
				    			</select></td>";
				  		$htm .= "</tr>";
				  		$i++;
					}
				} else {
					$htm .= "<table class='tabela' id='tabpro' style='width:100%; display:none;' align='center'>";
				}
		  		$htm .= "</table>";
		    	$htm .= "</td>";
		    	$htm .= "</tr>";
		  		$htm .= "</table>";
			}

	  		$this->htm = $htm;

		} else {

			$bgcolor = "#A8A8A8";

			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			$htm .= "<input type='hidden' name='identFormulario' id='identFormulario' value='1'>";
	    	$htm .= "<input type=\"hidden\" name=\"muncod_\" class=\"CampoEstilo\" id=\"muncod_\" value=\"".$arDados['muncod']."\" size=\"20\" maxlength=\"100\" />";
			$htm .= "<table class='tabela' style='width:{$percent}%;' align='center'>";
			$htm .= "<tr>";
			$htm .= "<td bgcolor='$bgcolor' colspan='50' align='center'><b>Endereço</b></td>";
			$htm .= "</tr>";

			$htm .= "<tr>";
	    	$htm .= "<td nowrap><b>2.1.1 CEP: <input type=\"text\" name=\"endcep1\" title=\"CEP\" onkeyup=\"this.value=mascaraglobal('##.###-###', this.value);\" onblur=\"getEnderecoPeloCEP(this.value,'1'); \" class=\"CampoEstilo\" id=\"endcep1\" value=\"".$arDados['precep']."\" size=\"13\" maxlength=\"10\" /><img src=\"../imagens/obrig.gif\" /> </b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>2.1.2 Logradouro: <input type=\"text\" title=\"Logradouro\" name=\"endlog\" class=\"CampoEstilo\" id=\"endlog1\" value=\"".$arDados['prelogradouro']."\" size=\"48\" /></b></td>";
	  		$htm .= "</tr>";
			$htm .= "<tr>";
	    	$htm .= "<td nowrap><b>2.1.3 Número: <input type=\"text\" name=\"endnum\" title=\"Número\" class=\"CampoEstilo\" id=\"endnum1\" value=\"".$arDados['prenumero']."\" size=\"5\" maxlength=\"8\" onkeypress=\"return somenteNumeros(event);\" /> </b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>2.1.4 Complemento: <input type=\"text\" name=\"endcom\" class=\"CampoEstilo\" id=\"endcom1\" value=\"".$arDados['precomplemento']."\" size=\"48\" maxlength=\"100\" /></b></td>";
	  		$htm .= "</tr>";
			$htm .= "<tr>";
	    	$htm .= "<td nowrap><b>2.1.5 Bairro: <input type=\"text\" title=\"Bairro\" name=\"endbai\" class=\"CampoEstilo\" id=\"endbai1\" value=\"".$arDados['prebairro']."\" /></b></td>";
	    	$htm .= "<td nowrap><b>2.1.6 Estado: <input type=\"text\" title=\"Estado\" name=\"estuf\" class=\"CampoEstilo\" id=\"estuf1\" value=\"".$arDados['estuf']."\" size=\"10\" /></b></td>";
			$htm .= "<td nowrap><b>2.1.7 Município: <input type=\"text\" name=\"mundescricao\" class=\"CampoEstilo\" id=\"mundescricao1\" value=\"".$arDados['muncod']."\" size=\"20\" maxlength=\"100\" /></b></td>";
	  		$htm .= "</tr>";

	    	$htm .= "</tr>";
	  		$htm .= "</table>";

	  		$this->htm = $htm;
		}

	}

	//Da echo no resultado do campo externo para alimentar o relatorio de respostas do questionario.
	//Sem essa função o relatório apenas exibirá o texto "Campo Externo".
	function retornoRelatorio( $qrpid, $perid ){

		$tp = $perid == 1528 ? 1 : 2;

		if( $tp == 2 ){
			$obQuest = new QuestoesPontuaisAnexos( );
			$dadosResp = $obQuest->pegaResposta( $qrpid, $perid );
			echo $dadosResp['arqdescricao'];
		} else {
			$obQuest = new QuestoesPontuaisEscolas( );
			//ver($obQuest->nomeEscolasCadastradas( $qrpid, $perid ), d);
			echo implode(", <br />", $obQuest->nomeEscolasCadastradas( $qrpid, $perid ) );
		}
	}

	//Função que salva o resultado do campo externo. Não deverá ser salvo no esquema do questionário, e sim no esquema do próprio sistema.
	function salvar() {

		if( $_POST['identExterno'] ){
			if( $_POST['queid'] == QUESTIONARIOPROMUN || $_POST['queid'] == QUESTIONARIOPROEST ){

			//	$arrPerid = array( 1735, 1736, 1737, 1738, 1739, 1740, 1741, 1742 );
				//require_once(APPRAIZ.'includes/classes/depuradorCodigo.class.inc');
				//$dbug 		= new DepuradorCodigo(true);

				//$dbug->dbug($_POST['perid'],'perid');
				if( $_POST['perid'] == PERGUNTA_1 || $_POST['perid'] == PERGUNTA_2 ){
					global $db;
					$v = 0;
					if( $_POST ){

						$sql = "SELECT qptid FROM par.questionarioprofuncionariotutor WHERE qrpid = ".$_POST['qrpid'];
						if( $db->pegaUm( $sql ) ){
							$sql = "DELETE FROM par.questionarioprofuncionariotutor WHERE qrpid = ".$_POST['qrpid'];
							$db->executar( $sql );
						}

						if($_POST['cpf']){
							foreach( $_POST['cpf'] as $chave => $valor ){
								if( $v == 0 ){
									if( $valor && $_POST['nome'][$chave] && $_POST['email'][$chave] && $_POST['ano'][$chave] && $_POST['emexercicio'][$chave] ){
										$sql = "INSERT INTO par.questionarioprofuncionariotutor ( qptcpf, qptnome, qptemail, qptano, qptexerc, qrpid, qptqnt ) VALUES ( '{$valor}', '{$_POST['nome'][$chave]}', '{$_POST['email'][$chave]}', '{$_POST['ano'][$chave]}', '{$_POST['emexercicio'][$chave]}', {$_POST['qrpid']}, {$_POST['quant']} )";
										$db->carregar( $sql );
									} else {
										echo "<script>alert('Faltam dados');</script>";
										$v = 1;
									}
								}
							}
						}
						else{
							echo "<script>alert('Informe o(s) CPF(s)!');</script>";
							$v = 1;
						}

						if( $v == 0 ){
							$db->commit();
						}
					}
				}
			} else {
				if( is_array($_POST['peridExterno']) ){
					foreach( $_POST['peridExterno'] as $perid ){
						$obQuest = new QuestoesPontuaisAnexos( );
	
						$obQuest->verifica( $_POST['qrpid'], $perid, $_POST['inuid'][$perid] );
	
						$campos	= array("qrpid"	        => $_POST['qrpid'],
										"perid"			=> $perid);
	
						$file = new FilesSimec("questoespontuaisanexos", $campos);
						if(is_file($_FILES["arquivo_".$perid]["tmp_name"])){
							$arquivoSalvo = $file->setUpload($_FILES["arquivo_".$perid]["name"], "arquivo_".$perid);
						}
					}
				}
			}

		} else {

			$obIdent = new Identificacao( );
			$obIdent->excluirPorQuestionarioFormulario( $_POST['qrpid'], $_POST['perid'] );
			$obIdent->commit();

			$obIdent = new Identificacao( );

			$_REQUEST['idtdtpreench'] = formata_data_sql($_REQUEST['idtdtpreench']);

			$arrCampos = array('perid', 'qrpid', 'idtmun', 'idtuf', 'idtdtpreench', 'idtresp', 'idtcargo', 'idtformacao', 'idtemailorg',
								  	'idttelorg', 'idtnomeorg', 'idtend', 'idtresppreench', 'idttel', 'idtemail', 'idtger');
			$obIdent->popularObjeto( $arrCampos );
			$obIdent->idtmun = trim($_POST['idtmun']);
			$obIdent->salvar();
			$obIdent->commit();
		}
	}

	//Função obrigatória. Dá echo no $this->htm para imprimir o campo externo na tela.
	function show(){
		echo $this->htm;
	}
}