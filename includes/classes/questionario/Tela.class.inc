<?php

class Tela {
	private $ger;
	private $js;
	private $table;
	private $styleParam;
	private $tableQuestionario;
	private $cellQuestionario;
	private $cellRodape;
	private $cellArvore;
	private $nivel;
	private $itemPergunta;
	private $tipoTela;
	private $subPergunta;
	private $arrDivSubPergunta;
	
	/*
	 * Monta a Tela principal do questionário
	 *
	 * Includes necessários: include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	 * 						 include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	 * Exemplo de como montá-lo: $tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => 'S') );
	 * 
	 * @param $arrParam com os seguintes parametros que podem ou não ser passados:
	 * qrpid 		- único parametro obrigatório. É o identificador de uma determinada resposta ao questionário. Cada entidade de resposta individual terá um. Ex: "qrpid" => $qrpid (qrpid atribuido anteriormente)
	 * tamDivArvore - Tamanho horizontal da árvore do questionário em porcentagem. Ex: 'tamDivArvore' => 25 (25 %)
	 * tamDivPx 	- Tamanho horizontal da árvore do questionário em pixels. 'tamDivPx' => 400 (400 px)
	 * habilitado	- Habilita/Desabilita o questionário. Precisa de parametro 'S' ou 'N' para habilitado e desabilitado respectivamente. Ex: 'habilitado' => 'S' (habilitado)
	 * relatorio	- Habilita/Desabilita a impressão de relatórios do questionário. Como parametro é passado a pagina onde o relatório será impresso a partir do modulo. Ex: 'relatorio' => 'modulo=relatorioQuestionario&acao=A'
	 * descricao	- Habilita/Desabilita a Descrição do questionário (com o ícone de ajuda). O parametro é um 'N' desabilitar, pois o default é habilitado. Ex: 'descricao' => 'N'
	 * 
	 * TODOS OS PARAMETROS, A NÃO SER O QRPID, SÃO OPCIONAIS. 
	 * 
	 * 
	 * PARA MONTAR UMA TABELA DENTRO DO QUESTIONARIO:
	 * 
	 * 1º: colocar o include necessário
	 * 
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";
	 * 
	 * 2º: colocar a condição para salvar os dados da tabela:
	 * 
	 *  if( $_POST['salvar_questionario'] && $_POST['idTabela'] ){
			$obMonta = new Montatabela();
			$obMonta->salvar();
		}
	 * 
	 * 
	 */
	
	function __construct(Array $arrParam){
		
		include_once APPRAIZ . "includes/classes/html/TElement.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTable.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTableRow.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTableCell.class.inc";
		include_once APPRAIZ . "includes/classes/html/TField.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCombo.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTextArea.class.inc";
		include_once APPRAIZ . "includes/classes/html/TText.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCheckGroup.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCheckButton.class.inc";
		include_once APPRAIZ . "includes/classes/html/TRadioGroup.class.inc";
		include_once APPRAIZ . "includes/classes/html/TRadioButton.class.inc";
		include_once APPRAIZ . "includes/classes/html/TData.class.inc";
		include_once APPRAIZ . "includes/classes/html/TLabel.class.inc";
		include_once APPRAIZ . "includes/classes/html/FormularioQuestionario.class.inc";
		
		include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
		include_once APPRAIZ . "includes/classes/questionario/QArvore.class.inc";
		include_once APPRAIZ . "includes/classes/questionario/JavaScript.class.inc";
		include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";

		if ( $_POST['ajax']){
			ob_clean();
		}

		$arrParam['qrpid'] = $_POST['qrpid'] ? $_POST['qrpid'] : $arrParam['qrpid'];
		$arrParam['queid'] = $_POST['queid'] ? $_POST['queid'] : $arrParam['queid']; 
		$arrParam['perid'] = $_REQUEST['perid'] ? $_REQUEST['perid'] : $arrParam['perid'];
		$arrParam['itpid'] = $_POST['itpid'] ? $_POST['itpid'] : $arrParam['itpid'];

		$this->styleParam['tamDivArvore'] = $arrParam['tamDivArvore'] ? $arrParam['tamDivArvore'] : 30;
		$this->styleParam['tamDivPx'] 	  = $arrParam['tamDivPx'] ? $arrParam['tamDivPx'] : 400;
		$this->styleParam['altDivPx'] 	  = $arrParam['altDivPx'] ? $arrParam['altDivPx'] : 510;
		$this->relatorio = $arrParam['relatorio'] ? $arrParam['relatorio'] : 'N';
		$this->descricao = $arrParam['descricao'];
		
		if ($arrParam['itpid']){
			$this->tipoTela = "questionario";
		}
		
		if ($_POST['salvar_questionario']){
			$arrParamSalv 						 = $arrParam;
			$arrParamSalv['salvar_questionario'] = $_POST['salvar_questionario'];
			$obQ = new GerenciaQuestionario($arrParamSalv);
			$obQ->salvar($_POST);
		}
		
		$this->ger 	 = new GerenciaQuestionario($arrParam);
		$this->arv 	 = new QArvore( array("questionario" => $this->ger->getQuestionario(), "perid" => $this->ger->getPeridPai(), "qrpid" => $arrParam['qrpid'], "relatorio" => $this->relatorio) );
		$this->js 	 = new JavaScript( $this->ger );
		
		$this->nivel 		= 0;
		$this->itemPergunta = array();
		
		$this->desab = $this->ger->setMensagemDesabilitada( $arrParam['msgDesabilitado'] );
		$this->desab = $this->ger->desabilitaQuestionario( $arrParam['habilitado'] );

		if ( $this->tipoTela != "questionario" ){
			self::montaTela();
		}else{
			self::montaTelaQuestionario();
		}

		if ( $_POST['ajax'] ){
			ob_flush();
			ob_end_clean();
			exit;			
		}
		$obQ = $this->ger->getQuestionario();
		
		if( EXCECAO == $obQ->queid ){
			die;
		}
	}
	
	private function imgNivel(){
		$img 		= new TElement( 'img' );
		$img->style = 'padding-left:' . $this->nivel * 10 . 'px;';
		$img->src 	= '../imagens/seta_filho.gif';
		$img->align = 'absmiddle';
		return $img;
	}
	

	private function imgAjuda( $ajuda ){
		$img 			  = new TElement( 'img' );
		$img->src 		  = '/imagens/ajuda.gif';
		$img->title 	  = $ajuda;
		return $img;
	}
	
	function montaTelaQuestionario(){
		$this->cellQuestionario = new TElement("div");
		self::questionario();
		
		// Gera linha do javascript
		$row 		= $this->tableQuestionario->addRow();		
		$row->style = "display : none;";
		$this->js->notInclude();
		$this->js->notFonte();
		$this->js->notParamJs();
		$row->addCell( $this->js );
		
		$this->cellQuestionario->show();
	}
	
	function montaTela(){
		$this->table   = new TTable;
		$tela 		   = $this->table;
		
		if ( !$_POST['ajax'] ){
			$tela 		   = new FormularioQuestionario();
			$div = new TElement('div');
			$div->id = 'telacentral';
			$div->add( $this->table );
			$tela->add( $div );
		}
		$this->table->border = 0;
		$this->table->height = ((int)$this->styleParam['altDivPx'] - 10);
		$row  		   		 = $this->table->addRow();
		// Gera célula da árvore
		$cell 			  = $row->addCell('');
		$cell->id 		  = 'telaarvore'; 
		$cell->rowspan 	  = 2; 
		$this->cellArvore = $cell; 
		// Gera célula do questionario
		$cell 		  			= $row->addCell(''); 
		$cell->id 	  			= 'telaquestionario';
		$cell->align  			= "center";
		$cell->valign 			= "top";
		$this->cellQuestionario = $cell; 
		// Gera célula do rodapé
		$row  		   	  = $this->table->addRow();
		$cell 		  	  = $row->addCell(''); 
		$cell->id 	  	  = 'telarodape';
		$cell->align  	  = "center";
		$cell->valign 	  = "bottom";
		$cell->style 	  = "padding-bottom:13px;";
		$this->cellRodape = $cell; 
		
		self::arvore();
		self::questionario();
		
		if ( $_POST['ajax'] ){
			$this->js->notInclude();
			$this->js->notFonte();
			$this->js->notParamJs();
			$this->js->atualizaPergunta();
			// Gera linha do javascript
			$row = $this->table->addRow();	
			$row->style = "display : none;";	
			$row->addCell( $this->js );
		}else{
			$tela->add( $this->js );
		}
		
		$tela->show();
	}


	private function arvore(){
		// conteúdo célula I 
		$fieldSet = new TElement("fieldset");
		$fieldSet->style = "overflow:auto; background:#FFF; height:".$this->styleParam['altDivPx']."px%; width:".$this->styleParam['tamDivPx']."px;";
		$legend = new TElement("legend");
		$legend->add( $this->arv->quebraTitulo( $this->ger->getQueTitulo() ) );
		$fieldSet->add( $legend );
		
		// Div externa
		$dI 		  = new TElement('div');
		$dI->id    = "bloco";
		$dI->class = "dtree";
		$dI->style = "overflow:auto; text-align:left; height:".((int)$this->styleParam['altDivPx'] - 10)."px; width:".$this->styleParam['tamDivPx']."px;";
		$p = new TElement('p');
		$aI = new TElement('a');
		$aI->href = 'javascript: arvore.openAll();';
		$aI->add('Abrir Todos');
		$aII = new TElement('a');
		$aII->href = 'javascript: arvore.closeAll();';
		$aII->add('Fechar Todos');
		$dII 		  = new TElement('div');
		$dII->id    = "_arvore";

		$p->add( $aI );
		$p->add(" &nbsp;|&nbsp; ");
		$p->add( $aII );
		
		$dI->add( $p );
		$dI->add( $dII );
		
		$fieldSet->add( $dI );
		$fieldSet->add( $this->arv->montaArvore() );
		$cell = $this->cellArvore;
		$cell->add( $fieldSet );
		$cell->valign = 'top';
		$cell->align  = 'center';
		$cell->style  = 'overflow:auto; width:'.$this->styleParam['tamDivArvore'].'%;';
	}

	private function questionario(){
		$this->tableQuestionario = new TTable;
		$this->tableQuestionario->valign = 'top';
		$this->cellQuestionario->add( $this->tableQuestionario );
		
		$arrObDados = (array) $this->ger->getDados();
		foreach($arrObDados as $obDados){
			if ( method_exists($this, 'tela' . simec_get_class( $obDados ) )){
				$this->{'tela' . simec_get_class( $obDados ) }( $obDados );
				$this->nivel++;
			}
		}
		
		if ( $this->tipoTela != "questionario" ){
			self::questionarioRodape();
		}
	}
		
	private function questionarioRodape(){
		
		$table = new TTable;
		$this->cellRodape->add($table);	
	
		$row 			= $table->addRow();
		$cell = $row->addCell('');
		
		$pergAnterior = $this->ger->anteriorPergunta();
		$pergProxima  = $this->ger->proximaPergunta();
		
		$div = new TElement('div');
		$div->style = "float:left; width:20%; text-align: left;";
		$botao = new TElement('input');
		if ( $pergAnterior ){
			$botao->onclick = "javascript: quest.atualizaTela(" . $pergAnterior->perid . ");";
		}else{
			$botao->disabled = "disabled";
		}
		$botao->class = 'btn btn-success';
		$botao->value = 'Anterior';
		$botao->type  = 'button';
		$botao->id    = 'anterior';
		$div->add($botao);
		$cell->add($div);		
		
		// Div/Botão II
		$div = new TElement('div');
		$div->style = "float: left; width:60%; text-align: center;";
		//Botão
		$botao = new TElement('input');

		if ( $pergAnterior ){
			if( $this->desab == false || $this->habilitado == 'N' ){
				$botao->disabled = "disabled";
			}else{
				$botao->onclick = "javascript: quest.salvar(" . $pergAnterior->perid . ");";
			}
		}else{
			$botao->disabled = "disabled";
		}
		$botao->class = 'btn btn-success';
		$botao->value = 'Salvar Anterior';
		$botao->type  = 'button';
		$botao->id    = 'salvar_anterior';
		$div->add($botao);
		
		$botao = new TElement('input');
		$botao->class = 'btn btn-success';
		$botao->onclick  = '';
		$botao->value 	 = 'Salvar';
		$botao->type  	 = 'button';
		$botao->id    = 'btn_qst_salvar';
		if( $this->desab == false || $this->habilitado == 'N' ){
			$botao->disabled = "disabled";
		}else{
			$botao->onclick  = 'quest.salvar(' . $this->ger->getPeridPai() . ');';
		}
		$div->add($botao);
		
		//Botão
		$botao = new TElement('input');
		if ( $pergProxima ){
			if( $this->desab == false || $this->habilitado == 'N' ){
				$botao->disabled = "disabled";
			}else{
				$botao->onclick = "javascript: quest.salvar(" . $pergProxima->perid . ");";
			}
		}else{
			$botao->disabled = "disabled";
		}
		$botao->class = 'btn btn-success';
		$botao->value = 'Salvar Próximo';
		$botao->type  = 'button';
		$botao->id    = 'salvar_proximo';
		$div->add($botao);
		$cell->add($div);
				
		// Div/Botão III
		$div = new TElement('div');
		$div->style = "float:right; width:20%; text-align: right;";
		//Botão
		$botao = new TElement('input');
		if ( $pergProxima ){
			$botao->onclick = "javascript: quest.atualizaTela(" . $pergProxima->perid . ");";
		}else{
			$botao->disabled = "disabled";
		}
		$botao->class = 'btn btn-success';
		$botao->value = 'Próximo';
		$botao->type  = 'button';
		$botao->id    = 'proximo';
		$div->add($botao);
		$cell->add($div);		
		
	}
	
	private function telaQPergunta( QPergunta $obj){
		// Inclui pergunta ao JavaScript
		$pergPai = $this->ger->perguntaPai( array("perid" => $obj->perid) );
		$this->js->addPergunta($obj, ($pergPai instanceof QPergunta ? $pergPai->perid : "" ), $this->ger->itemPai($obj) );
		
	//	ver($obj->perid, $obj->queid, $this->ger->getQrpid(), d);
		
		//verifica se tem alguma condição bloqueando ele.
		$habilitado = '';
		$justificativa = $this->ger->verificaCondicao($obj->perid, $obj->queid, $this->ger->getQrpid());

		
		if ( $this->arrDivSubPergunta[$this->ger->itemPai($obj)] instanceof TElement ){
			$table = new TTable;
			$this->arrDivSubPergunta[$this->ger->itemPai($obj)]->style = "margin-top:5px;";
			$this->arrDivSubPergunta[$this->ger->itemPai($obj)]->add( $table );
		}else{
			$table = $this->tableQuestionario;
		}
		
		if( $justificativa ){
			$habilitado = 'N';
			$this->habilitado = $habilitado;
			// linha 0
			$row 			= $table->addRow();
			$cell 			= $row->addCell( '<font color=red>Pergunta Desabilitada: </font>'.$justificativa );
			$cell->class	= "SubTituloEsquerda";
			$imagem 		= $obj->perajuda ? self::imgAjuda( $obj->perajuda ) : '';
			$cell->add( $imagem );
		}
		 
		// verifico se ele tem alguma condição
		$funcao = $this->ger->verificaFuncao($obj->perid,$this->ger->getQrpid());

		if( $funcao == false ){
			$habilitado = 'N';
			$this->habilitado = $habilitado;
			// linha 0
			$row 			= $table->addRow();
			$cell 			= $row->addCell( '<font color=red>Pergunta Desabilitada</font>' );
			$cell->class	= "SubTituloEsquerda";
			$imagem 		= $obj->perajuda ? self::imgAjuda( $obj->perajuda ) : '';
			$cell->add( $imagem );
		}
		
		// linha I
		$row 			= $table->addRow();
		$cell 			= $row->addCell( $obj->pertitulo . ' ' );
		$cell->class	= "SubTituloEsquerda";
		$imagem 		= $obj->perajuda ? self::imgAjuda( $obj->perajuda ) : '';
		$cell->add( $imagem ); 
		// linha II
		$row 			= $table->addRow();
		
 		switch ($obj->pertipo){
 			case 'RA':
 				$row->addCell( $this->campoRadio( $obj, $habilitado ) );
 				break;
 			case 'CK':
 				$row->addCell( $this->campoCheck( $obj, $habilitado ) );
 				break;
 			case 'TX':
 				$row->addCell( $this->campoText( $obj, $habilitado ) );
 				break;
 			case 'TA':
 				$row->addCell( $this->campoTextArea( $obj, $habilitado ) );
 				break;
 			case 'CB':
 				$cell = $row->addCell( $this->campoCombo( $obj, $habilitado ) );
 				$this->inserirDivSubPerg($cell, $obj);
 				break;
 			case 'CM':
 				$cell = $row->addCell( $this->campoComboMultiplo( $obj, $habilitado ) );
 				$this->inserirDivSubPerg($cell, $obj);
 				break;
 			case 'DT':
 				$row->addCell( $this->campoData( $obj, $habilitado ) );
 				break;
 			case 'EXT':
 				$cell = $row->addCell( $this->campoExternoTabela( $obj ) );
 				$cell->add( $this->campoHidden( $obj ) );
 				break;
 			case 'EXF':
 				$cell = $row->addCell( $this->campoExternoFormulario( $obj ) );
 				$cell->add( $this->campoHidden( $obj ) );
 				break;
 		}
	}
	
	private function campoTextArea( QPergunta $obj, $habilitado = null ){
		$campo = new TTextArea("perg_{$obj->perid}");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setMaxlength( $obj->pertamanho );
		$campo->setObrig( $obj->perobrigatorio == "t" );
		$h = $habilitado == 'N' || $this->desab == false ? false : true;
		$campo->setHabil( $h );
		
		return $campo;
		
	}
	
	private function campoText( QPergunta $obj, $habilitado = null ){
		$campo = new TText("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setMascara( $obj->permascara );
		$campo->setTamanho( $obj->pertamanho );
		$campo->setObrig( $obj->perobrigatorio == "t" );
		$h = $habilitado == 'N' || $this->desab == false ? false : true;
		$campo->setHabil( $h );
		
		return $campo;
	}
	
	private function campoExternoTabela( QPergunta $obj, $habilitado = null ){
		$campo = new Montatabela( );		
		$h = $habilitado == 'N' || $this->desab == false ? 2 : 0;
		$campo->montaNovaTabela( $obj->perid, $this->ger->getQrpid(), null, $h );
				
		return $campo;
		
	}
	
	private function campoExternoFormulario( QPergunta $obj ){
		
		$campo = new CampoExternoControle( );		
		$campo->montaNovoCampo( $obj->perid, $this->ger->getQrpid() );
				
		return $campo;
		
	}
	
	private function campoHidden( QPergunta $obj ){
		
		$hidden = new TElement('input');
		$hidden->type  = 'hidden';
		$hidden->value  = true;
		$hidden->name  = 'perg['.$obj->perid.']';
		$hidden->id    = 'perg['.$obj->perid.']';
				
		return $hidden;
		
	}
	
	private function campoData( QPergunta $obj, $habilitado = null ){
		$campo = new TData("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setLabel( $obj->perajuda );
		$campo->setObrig( $obj->perobrigatorio == "t" );
		$campo->setMascara( $obj->permascara );
		$h = $habilitado == 'N' || $this->desab == false ? false : true;
		$campo->setHabil( $h );
		
		return $campo;
	}
	
	private function campoCombo( QPergunta $obj, $habilitado = null ){
		$campo = new TCombo("perg[{$obj->perid}]");
		
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setAcao("javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : this.value}, this)");
		$h = $habilitado == 'N' || $this->desab == false ? false : true;
		$campo->setHabil( $h );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo) );
		}
		return $campo;		
	}
	
	private function campoComboMultiplo( QPergunta $obj, $habilitado = null ){
		
		$campo = new TCombo("perg[{$obj->perid}]");
		$campo->setMultiplo(true);
		$campo->setOpcao(null);
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setAcao("javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : this.value}, this)");
		$h = $habilitado == 'N' || $this->desab == false ? false : true;
		$campo->setHabil( $h );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo) );
		}
		return $campo;		
	}
	
	private function campoRadio( QPergunta $obj, $habilitado = null ){
		
		$campo = new TRadioGroup("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$h = $habilitado == 'N' || $this->desab == false ? false : true;
		$campo->setHabil( $h );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			unset($onclick, $divSubPerg);
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
				$onclick = "javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : {$obItem->itpid}}, this)";
			}else{
				$onclick = "javascript: quest.closeSubPerguntas({$obj->perid});";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo, "onClick" => $onclick, "divSubPergunta" => $divSubPerg ));
		}
		return $campo;		
	}
	
	private function campoCheck( QPergunta $obj, $habilitado = null ){
		
		$campo 		= new TCheckGroup("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$h = $habilitado == 'N' || $this->desab == false ? false : true;
		$campo->setHabil( $h );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			unset($onclick, $divSubPerg);
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
				$onclick = "javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : {$obItem->itpid}}, this)";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo, "onClick" => $onclick, "divSubPergunta" => $divSubPerg ));
		}
		return $campo;		
	}
	
	private function telaQQuestionario($obj){
		if ( is_null($this->subPergunta) ){
			$table = $this->tableQuestionario;	
		}
		
		$row 			= $table->addRow();
		$cell 			= $row->addCell( $obj->quetitulo. ' ' );
		if( $this->descricao != 'N' ){
			$imagem 		= $obj->quedescricao ? self::imgAjuda( $obj->quedescricao ) : '';
			$cell->add( $imagem );
		}
		$cell->class	= "SubTituloEsquerda";
		$cell->colspan 	= 2;
	}
	
	private function telaQGrupo($obj){
		if ( is_null($this->subPergunta) ){
			$table = $this->tableQuestionario;	
		}else{
			$table = $this->divSubPergunta[$this->ger->itemPai($obj)];
		}
		
		$row 			= $table->addRow();
		$cell 			= $row->addCell( self::imgNivel() );
		$cell->class	= "SubTituloEsquerda";
		$cell->colspan 	= 2;
		$cell->add( $obj->grptitulo. ' ' );
		$imagem 		= $obj->grpajuda ? self::imgAjuda( $obj->grpajuda ) : '';
		$cell->add( $imagem );
	}

	private function criaDivSubPerg($itpid, $idDiv){
		$divSubPerg 	   = new TElement("div");
        $divSubPerg->id    = $idDiv;
        $divSubPerg->style = "display:none; margin-top:5px;";
        $divSubPerg->add('');			

		$this->arrDivSubPergunta[$itpid] = $divSubPerg;
		
		return $divSubPerg; 
	}
	
	private function inserirDivSubPerg(TElement $cell, QPergunta $obPerg){
		$arrObItens = $this->ger->carregaObItensPergunta($obPerg);
		foreach($arrObItens as $obItem ){
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obPerg->perid}_{$obItem->itpid}");
				$cell->add( $divSubPerg );
			}
		}	
	}

}
?>		
