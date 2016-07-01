<?php
// include_once APPRAIZ ."includes/workflow.php";

class CampoExternoControle{

	private $htm;
	private static $camposDefault;

	//Função obrigatória. Nessa função deverá ser montado a questão que aparecerá dentro do campo externo. Deverá ser colocada dentro do $this->htm.
	public function montaNovoCampo( $perid, $qrpid, $percent = 90 ){

		$obQestionario = new QQuestionarioResposta();

		$queid = $obQestionario->pegaQuestionario( $qrpid );

		$obQuestAnexo = new Par3_Model_QuestoesEstrategicasAnexos( );
		$dadosResp    = $obQuestAnexo->carregarResposta( $qrpid, $perid );

		$htm = '';
		if (in_array($perid, array(3963))) {
    		$obQuestEscolaCE = new Par3_Model_QuestoEsestrategicasEscolasCe();
    		$qdtEscolas      = $obQuestEscolaCE->retornarQtdEscolas( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas com CE em funcionamento
    		         <i class=\"fa fa-pencil-square-o escolas_ce\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escola da redemunicipal de ensino: <span id=total_escolas_ce >$qdtEscolas</span>";
		} else if (in_array($perid, array(4022))) {
    		$obQuestEscolaGremio = new Par3_Model_QuestoEsestrategicasEscolasGremio();
    		$qdtEscolas      = $obQuestEscolaGremio->retornarQtdEscolas( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas com Grêmio em funcionamento
    		         <i class=\"fa fa-pencil-square-o escolas_gremio\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escola da redemunicipal de ensino: <span id=total_escolas_gremio >$qdtEscolas</span>";
		} else if (in_array($perid, array(3929))) {
    		$htm .= " <div clas=\"ibox-content\">
    		                  O Plano Nacional de Educação (PNE), instituído pela Lei nº 13.005/2014,
    		              determinou, para o primeiro ano de vigência, a elaboração ou adequação dos
    		              planos estaduais, distrital e municipais de educação, em consonância com o
    		              texto nacional. No dia 24 de junho de 2015 venceu o prazo para todas as
    		              prefeituras, estados e o Distrito Federal finalizarem seus planos
    		              de educação, ou adequarem os planos já aprovados em lei às diretrizes
    		              do PNE. Se o seu estado/município ainda não publicou a lei com o plano
    		              de educação, poderá entrar em contato com a SASE (Secretaria de Articulação
    		              com os Sistema de Ensino), pelo e-mail XXXXXXXX, ou telefone(s) XXXXXXXXXX
    		          </div>";
		} else if (in_array($perid, array(3930))) {
			$htm .= " Arquivo em anexo: <a onclick=\"janela(window.location.href+'&req=download&arqid={$dadosResp['arqid']}', 600, 480);\">
			             {$dadosResp['arqdescricao']}</a><br />";
            if (!$dadosResp['arqid']) {
        		$htm .= " <div clas=\"ibox-content\" style=\"color: red\">";
        		$htm .= "         Caso o estado/município possua o plano Estadual/Municipal de Educação
        		              com a sua lei devidamente sancionada e o sistema não mostre o arquivo anexado,
        		              deverá entrar em contato com a equipe responsável na SASE(Secretaria de Articulação
        		              com os Sistemas de Ensino), pelo e-mail XX, ou telefone (x) xx";
        		$htm .= " </div>";
            }
		} else {
    		if( $dadosResp['arqid'] ){
    			if (true) {
    				$boExcluir = "<a href=\"#\" onclick=\"excluirArquivo( ".$dadosResp['arqid'].", ".$perid." );\" title=\"Excluir Anexo\">
    				                    <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\">
    				                </a>";
    			} else {
    				$boExcluir = "<a href=\"#\" title=\"Excluir Anexo\">
    				                    <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">
    				                </a>";
    			}
    			$htm .= $boExcluir." Arquivo em anexo: <a onclick=\"janela(window.location.href+'&req=download&arqid={$dadosResp['arqid']}', 600, 480);\">".$dadosResp['arqdescricao']."</a>";
    		} else {
        		$htm .= "<input type='hidden' name='salvar_questionario' id='salvar_questionario'     value='1'>";
        		$htm .= "<input type='hidden' name='perid'               id='perid'                   value=''>";
        		$htm .= "<input type='hidden' name='req'                                              value='salvarArquivoQuestionario'>";
        		$htm .= "<input type='hidden' name='qrpid'               id='qrpid'                   value='{$qrpid}'>";
        		$htm .= "<input type='hidden' name='peridExterno[]'      id='peridExterno_{$perid}'   value='{$perid}'>";
        		$htm .= "<input type='hidden' name='arqid[{$perid}]'     id='arqid'                   value='{$dadosResp['arqid']}'>";
    			$htm .= "<input type='file' name='arquivo_{$perid}' id='arquivo_itpid_{$itpid}' />";
    		}
		}

  		$this->htm = $htm;

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
		if( is_array($_POST['peridExterno']) ){
			foreach( $_POST['peridExterno'] as $perid ){
				$obQuest = new Par3_Model_QuestoesEstrategicasAnexos();

				if( $_FILES["arquivo_".$perid]["name"] ){
					$obQuest->verifica( $_POST['qrpid'], $perid, $_POST['inuid'][$perid] );

					$campos	= array("qrpid"	        => $_POST['qrpid'],
									"perid"			=> $perid);

					$file = new FilesSimec("questoesestrategicasanexos", $campos);
					if(is_file($_FILES["arquivo_".$perid]["tmp_name"])){
						$arquivoSalvo = $file->setUpload($_FILES["arquivo_".$perid]["name"], "arquivo_".$perid);
					}
				}
			}
		}
	}

	//Função obrigatória. Dá echo no $this->htm para imprimir o campo externo na tela.
	function show(){
		echo $this->htm;
	}
}