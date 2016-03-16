<?php

class Lista{
	
	const TYPENUMERIC 		= 'numeric'; 		// 2.345,34 | 0
	const TYPENUMERICNATIVO = 'numericnativo';  // 2345.34
	const TYPEMONEY 		= 'money'; 			// 2.345,34 | 0,00
	const TYPESTRING 		= 'string'; 		// texto comum
	const TYPEDATE	 		= 'date'; 			// 12-10-2010
	const TYPETIME	 		= 'time'; 			// 12:59
	
	private $table;
	private $row;
	private $cell;
	private $rowspan;
	private $mapRow;
	private $arCabecalho;
	private $arCorpo;
	private $arCorpoTotal;
	private $acao;
	private $classTr;
	private $arHTML;
	private $config;
	private $totalColuna;
	
	/**
	 * Atributos usados somente quando usar paginação ajax 
	 */
	private $boPaginacao;
	private $nmDiv;
	private $arConfigPaginacao;
	
	public function __construct(Array $arParam = null, $arDefinicaoPaginacao = array()){
		self::open( $arParam );
		$this->rowspanTotal = 1;
		
		// Define se a listagem conterá um totalizador de registros
		$this->config['totalRegistro'] = $arParam['totalRegistro'] ? $arParam['totalRegistro'] : false; 
		// Define se a listagem conterá um totalizador das linhas
		$this->config['totalLinha'] = $arParam['totalLinha'] ? $arParam['totalLinha'] : false; 
		// Define se a listagem conterá uma subLinha para cada linha
		$this->config['subLinha'] = $arParam['subLinha'] ? $arParam['subLinha'] : false; 
		//Será utilizado para o cálculo do total de colunas
		$this->acaoAltTotalColuna = 0;
		
		/**
		 * Bloco referente a paginação ajax
		 */
		$this->setNmDiv($arDefinicaoPaginacao['nmDiv']);
		$this->setPaginacao($arDefinicaoPaginacao['boPaginacao']);
		if($this->boPaginacao){
			if(!$this->nmDiv){
				echo "<script type=\"text/javascript\">alert('É necessário passar o nome da DIV de retorno.');</script>";
			} else {
				
				if($arDefinicaoPaginacao['filtros']){
					$arBusca = array('","', '{"', '":"');
					$arSubs  = array('"}, {"', '{name:"', '", value:"');
					$arDefinicaoPaginacao['filtros'] = "[" . str_replace($arBusca, $arSubs, $arDefinicaoPaginacao['filtros']) . "]";					
				}
				
				?>
				<script type="text/javascript">

					function listaPaginacao(obj,
											nrPaginaAtual, 
											nrRegPorPagina, 
											nrBlocoAtual, 
											nmDiv, 
											nmControleMetodo, 
											nrBlocoPaginacaoMaximo ) {

						var data = new Array();
							data = eval('<?php echo $arDefinicaoPaginacao['filtros']; ?>');

						if(!nrPaginaAtual){
							nrPaginaAtual = 1;
						}
						
						url = '/includes/montaListaAjax/listaPaginacaoAjax.php';
						data.push({name : 'nmDiv', 					value : nmDiv},
								  {name : 'nrPaginaAtual', 			value : nrPaginaAtual},
								  {name : 'nrRegPorPagina', 		value : nrRegPorPagina},
								  {name : 'nrBlocoAtual', 			value : nrBlocoAtual},
								  {name : 'nmControleMetodo', 		value : nmControleMetodo},
								  {name : 'nrBlocoPaginacaoMaximo', value : nrBlocoPaginacaoMaximo}
								 );
						 
						jQuery.ajax({
							type	: 'POST',
							async	: false,
							beforeSend: function(){
								//jQuery('#'+nmDiv).fadeTo("fast", 0.3);
								divCarregando(obj);
							},
							url		: url,
							data	: data,
							success	: function(msg){
								//jQuery('#'+nmDiv).fadeTo("slow", 1);
								jQuery('#'+nmDiv).html(msg);
								divCarregado();
							}
							
						});					
					}
				</script>			
				<?php 
			}			
		}
	}
	
	public function setPaginacao($boPaginacao = false){
		$this->boPaginacao = $boPaginacao;
	}
	
	public function setNmDiv($nmDiv = ""){
		$this->nmDiv = $nmDiv;
	}
	
	public function setCabecalho(Array $arCabecalho = null){
		$this->arCabecalho = $arCabecalho;
	}
	
	public function setAcao( $acao ){
		$this->acao = $acao;
	}
	
	public function setClassTr( $classTr ){
		$this->classTr = $classTr;
	}
	
	public function setCorpo($arDados = null, Array $arConfig = null){
		$this->arCorpo['dados']   = $arDados;
		$this->arCorpo['config']  = $arConfig;
	}

	public function show($arConfigPaginacao = array()){
		if ( is_array($this->arCorpo['dados']) || $this->arCorpo['dados'] ){
			self::montaCabecalho();
		}
		$this->arConfigPaginacao = $arConfigPaginacao;
		self::montaCorpo();
		$this->table->show();
	}	
	
	private function open(Array $arParam = null){
		$this->table = new TTable();
		$this->table->cellspacing = "0"; 
		$this->table->cellpadding = "2"; 
		$this->table->border	  = "0";
		$this->table->align		  = "center"; 
		$this->table->style		  = $arParam['style'];
		$this->table->bgcolor	  = "";
		$this->table->class		  = "listagem";
	}	
	
	private function montaCabecalho(Array $arCabecalho = null){
		$arCabecalho = !is_null( $arCabecalho ) ? $arCabecalho : $this->arCabecalho;
		
		if ( !is_null($arCabecalho) ){
			if( is_null($this->cell) ){
				self::atribuiNivelCabecalho( $arCabecalho, 1 );
			}		
			
			foreach( $arCabecalho as $indice => $cabecalho ){
				if ( is_array($cabecalho) ){
					$txtCabecalho = $indice;
				}else{
					$txtCabecalho = $cabecalho;
				}
				$txtCabecalho 								  = is_array($cabecalho) ? $indice : $cabecalho;
				$this->row[$this->cell[$txtCabecalho]['row']] = (($this->row[$this->cell[$txtCabecalho]['row']] instanceof TElement) 
																? 
																	$this->row[$this->cell[$txtCabecalho]['row']] 
																: 
																	$this->table->addRow());
				$row										  = $this->row[$this->cell[$txtCabecalho]['row']];
				$row->bgcolor = '#e9e9e9';
				
				$this->cell[$txtCabecalho]['obj'] = $row->addCell( $txtCabecalho );
				$this->cell[$txtCabecalho]['obj']->align  = 'center';
				$this->cell[$txtCabecalho]['obj']->valign = 'middle';
				$this->cell[$txtCabecalho]['obj']->class  = 'title';
				$this->cell[$txtCabecalho]['obj']->style  = 'border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;';
				
				if ( is_int($this->cell[$txtCabecalho]['colspan']) ){
					$this->cell[$txtCabecalho]['obj']->colspan = $this->cell[$txtCabecalho]['colspan'];
				}
				
				if ( !is_array( $cabecalho ) ){
					// Soma 1 ao total do cabecalho, pois quando entra aqui é o último nível do cabeçalho
					$this->totalColuna++;
					$rowspan = $this->cell[$txtCabecalho]['row'] != 1 ? ($this->rowspanTotal - ($this->cell[$txtCabecalho]['row'] - 1)) : $this->rowspanTotal;
					$this->cell[$txtCabecalho]['obj']->rowspan = $rowspan;
//					$this->cell[$txtCabecalho]['obj']->add( '(' . $this->rowspanTotal . ' - ' . $this->cell[$txtCabecalho]['row'] . ')' );
				}else{
					self::montaCabecalho( $cabecalho );
				}	
			}
		}
	}
	
	private function montaPaginacaoNovo()
	{
		extract($this->arConfigPaginacao);
		//ver($this->arConfigPaginacao,d);
		/**
		 * BLOCO PAGINAÇÃO 
		 */
		$anterior = $nrPaginaAtual - 1;
		$proximo  = $nrPaginaAtual + 1;
		
		# Numero total de páginas
		$nrPaginas = ceil($nrTotalRegistro / $nrRegPorPagina);
		
		# Pegamos a Página Inicial do Bloco da Paginação
		$nrPagInicial = ($nrBlocoAtual*$nrBlocoPaginacaoMaximo)-($nrBlocoPaginacaoMaximo);
		
		# Pegamos a Página Final do Bloco da Paginação
		$nrPagFinal	  = ($nrPagInicial+$nrBlocoPaginacaoMaximo);
		$nrPagFinal   = $nrPagFinal <= $nrPaginas ? $nrPagFinal : $nrPaginas;
		
		$imgProximo = "<img src=\"../imagens/controle_paginacao/proximo.gif\" align=\"top\" />";
		$imgProximo_01 = "<img src=\"../imagens/controle_paginacao/proximo_01.gif\" align=\"top\" />";
		$imgAnterior = "<img src=\"../imagens/controle_paginacao/anterior.gif\" align=\"top\" />";
		$imgAnterior_01 = "<img src=\"../imagens/controle_paginacao/anterior_01.gif\" align=\"top\" />";
		$imgBlocoAnterior = "<img src=\"../imagens/controle_paginacao/bloco_anterior.gif\" align=\"top\" />";
		$imgBlocoAnterior_01 = "<img src=\"../imagens/controle_paginacao/bloco_anterior_01.gif\" align=\"top\" />";
		$imgBlocoProximo = "<img src=\"../imagens/controle_paginacao/bloco_proximo.gif\" align=\"top\" />";
		$imgBlocoProximo_01 = "<img src=\"../imagens/controle_paginacao/bloco_proximo_01.gif\" align=\"top\" />";
		
		# Verificamos se a quantidade de páginas é maior que 1
		if($nrPaginas > 1 ) {
			# Se tem página antes da atual, mostramos o botão de anterior
		    if($anterior > 0) {
				$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$anterior.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgAnterior.'</a>';		    		
		    } else { # mostra o botão anterior desabilitado
		    	$html .= $imgAnterior_01;
		    }
		    
		    if($nrPaginaAtual < 5){
		    	$nrPagInicial = 0;		    	
		    } else {
		    	$nrPagInicial = $nrPaginaAtual - 6;		    	
		    }
		    $nrPagFinal = $nrPaginaAtual + 5;
		    
		    # Listando as paginas
			for($i=($nrPagInicial+1);$i <= ($nrPagFinal);$i++) {
				$sublinhado = "text-decoration: underline;";
				if($i == $nrPaginaAtual) {
					$marcar = "background:black;color:white;padding-left:3px;padding-right:3px;";
					$sublinhado = "";
				}
				
				$pipe = ' <span style=\'color:#838B8B\' > | </span>';
				if($i == $nrPaginas){
					$pipe = "";
				}
				
				$html .= ' <a style="cursor:pointer;'.$sublinhado.$marcar.'" onclick="listaPaginacao(this, '.$i.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$i.'</a> '.$pipe.'';
				unset($marcar);
			}
			
			# verifica se existe página a frente da atual, se sim aparece o botão de próximo
			if($proximo <= $nrPaginas) {
				$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$proximo.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgProximo.'</a>';					
			} else { # desabilita o botão proximo
				$html .= $imgProximo_01;
			}
		}
		
		$totalColuna = $this->totalColuna;
		// Caso possua "Ação" e NÃO possua "Cabeçalho" aumenta 1 do total de colunas.
		$totalColuna += ($this->acao && empty($this->arCabecalho)) ? 1 : 0; 
		$qtdCol = $totalColuna;
		$table  = $this->table;
		$row 	= $table->addRow();
		//$cell 	= $row->addCell('Exibindo: '.$nrTotalRegistro.' registros em  páginas '. $html);
		$cell 	= $row->addCell($html);
		$cell->colspan = $qtdCol; 
		$cell->nowrap  = 'nowrap'; 
		$cell->style   = 'border-top: 2px solid #000000; font-weight: bold; text-align: right';
	}
	
	private function montaPaginacao()
	{
		extract($this->arConfigPaginacao);
		/**
		 * BLOCO PAGINAÇÃO 
		 */
		$anterior = $nrPaginaAtual - 1;
		$proximo  = $nrPaginaAtual + 1;
		
		# Numero total de páginas
		$nrPaginas = ceil($nrTotalRegistro / $nrRegPorPagina);
		
		# Pegamos a Página Inicial do Bloco da Paginação
		$nrPagInicial = ($nrBlocoAtual*$nrBlocoPaginacaoMaximo)-($nrBlocoPaginacaoMaximo);
		
		# Pegamos a Página Final do Bloco da Paginação
		$nrPagFinal	  = ($nrPagInicial+$nrBlocoPaginacaoMaximo);
		$nrPagFinal   = $nrPagFinal <= $nrPaginas ? $nrPagFinal : $nrPaginas;
		
		$imgProximo = "<img src=\"../imagens/controle_paginacao/proximo.gif\" align=\"top\" />";
		$imgProximo_01 = "<img src=\"../imagens/controle_paginacao/proximo_01.gif\" align=\"top\" />";
		$imgAnterior = "<img src=\"../imagens/controle_paginacao/anterior.gif\" align=\"top\" />";
		$imgAnterior_01 = "<img src=\"../imagens/controle_paginacao/anterior_01.gif\" align=\"top\" />";
		$imgBlocoAnterior = "<img src=\"../imagens/controle_paginacao/bloco_anterior.gif\" align=\"top\" />";
		$imgBlocoAnterior_01 = "<img src=\"../imagens/controle_paginacao/bloco_anterior_01.gif\" align=\"top\" />";
		$imgBlocoProximo = "<img src=\"../imagens/controle_paginacao/bloco_proximo.gif\" align=\"top\" />";
		$imgBlocoProximo_01 = "<img src=\"../imagens/controle_paginacao/bloco_proximo_01.gif\" align=\"top\" />";
		
		# Verificamos se a quantidade de páginas é maior que 1
		if($nrPaginas > 1 ) {
			//$html .= "<br />";
			
			# Se o bloco atual é maior que 1, aparece o botão para voltar um bloco 
			if($nrBlocoAtual > 1){
				$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$nrPagInicial.','.$nrRegPorPagina.','.($nrBlocoAtual-1).',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\' )">'.$imgBlocoAnterior.'</a>';
			} else { # desabilita o botão voltar um bloco
				$html .= $imgBlocoAnterior_01;
			}
			
			# Se tem página antes da atual, mostramos o botão de anterior
		    if($anterior > 0) {
		    	# Se a Página Atual for igual a página incial do bloco o botão anterior tem que mudar de bloco
		    	if($nrPaginaAtual == ($nrPagInicial+1)){
		    		$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$anterior.','.$nrRegPorPagina.','.($nrBlocoAtual-1).',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgAnterior.'</a>';
		    	} else {
					$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$anterior.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgAnterior.'</a>';		    		
		    	}
		    } else { # mostra o botão anterior desabilitado
		    	$html .= $imgAnterior_01;
		    }
		    
		    # Listando as paginas
			for($i=($nrPagInicial+1);$i <= ($nrPagFinal);$i++) {
				$sublinhado = "text-decoration: underline;";
				if($i == $nrPaginaAtual) {
					$marcar = "background:black;color:white;padding-left:3px;padding-right:3px;";
					$sublinhado = "";
				}
				
				$pipe = ' <span style=\'color:#838B8B\' > | </span>';
				if($i == $nrPaginas){
					$pipe = "";
				}
				
				$html .= ' <a style="cursor:pointer;'.$sublinhado.$marcar.'" onclick="listaPaginacao(this, '.$i.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$i.'</a> '.$pipe.'';
				unset($marcar);
			}
			
			# verifica se existe página a frente da atual, se sim aparece o botão de próximo
			if($proximo <= $nrPaginas) {
				# Se a Página Atual for igual a página final do bloco o botão próximo tem que mudar de bloco, senão só muda para a próxima página
				if($nrPaginaAtual == ($i-1)){
					$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$proximo.','.$nrRegPorPagina.','.($nrBlocoAtual+1).',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgProximo.'</a>';					
				} else {
					$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$proximo.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgProximo.'</a>';					
				}
				
			} else { # desabilita o botão proximo
				$html .= $imgProximo_01;
			}
		}
		
		# Verifica se a página final e menor que o total de páginas se for, aparece o botão de avançar bloco
		if($nrPaginas > $nrBlocoPaginacaoMaximo && $nrPagFinal < $nrPaginas){
			$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.($i).','.$nrRegPorPagina.','.($nrBlocoAtual+1).',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgBlocoProximo.'</a>';
		} elseif($nrPaginas > 1) { # desabilita o botão avançar bloco
			$html .= $imgBlocoProximo_01;	
		}
		
		//$cell 	= $row->addCell('Exibindo: '.($nrPosInicial+1).' a '.($nrPagFinal).' de '.$nrTotalRegistros.' registros em '.$nrTotalPag.' páginas '. $html);
		$totalColuna = $this->totalColuna;
		// Caso possua "Ação" e NÃO possua "Cabeçalho" aumenta 1 do total de colunas.
		$totalColuna += ($this->acao && empty($this->arCabecalho)) ? 1 : 0; 
		$qtdCol = $totalColuna;
		$table  = $this->table;
		$row 	= $table->addRow();
		//$cell 	= $row->addCell('Exibindo: '.$nrTotalRegistro.' registros em  páginas '. $html);
		$cell 	= $row->addCell($html);
		$cell->colspan = $qtdCol; 
		$cell->nowrap  = 'nowrap'; 
		$cell->style   = 'border-top: 2px solid #000000; font-weight: bold; text-align: right';
	}
	
	private function montaCorpo(){
		$arDados  = $this->arCorpo['dados'];
		$arConfig = $this->arCorpo['config'];
		$table 	  = $this->table;
		
		if ( !is_array($arDados) || !$arDados ){
			$row  = $table->addRow();
			$cell = $row->addCell('Não foram encontrados registros.');
			$cell->style = 'color:red;';
			return;
		}
		
		// Define o total de colunas a serem montadas na listagem
		$this->totalColuna = ( !is_null($this->totalColuna) ? $this->totalColuna : count( $arDados[0] ) );
		// Se tiver acao a coluna extra "Ação" deve ser contabilizada.
//		if ( $this->acao )
//		$this->totalColuna++;

		$contLinha = 0;
		$totalColuna = $this->totalColuna;
		// Caso possua "Ação" e "Cabeçalho" diminui 1 do total de colunas.
		$totalColuna += ($this->acao && $this->arCabecalho) ? -1 : 0; 
		
		foreach( $arDados as $linha ):
			$color 		  	  = is_int($contLinha/2) ? '' : '#f7f7f7';
			$row 	  	  	  = $table->addRow();
			if($this->classTr){
				$row->class = $this->classTr;				
			}
			$row->bgcolor 	  = $color;
			$row->onmouseover = "this.bgColor='#ffffcc'";
			$row->onmouseout  = "this.bgColor='{$color}'";
			
			if ( $this->acao ){
				$coluna = self::carregaHTML($this->acao, $linha);
				$row->addCell( $coluna );
//				$this->acaoAltTotalColuna = 1;
			}
			$i = 0;
			foreach( $linha as $indColuna => $coluna ):
				// define tipo de dado
				$type = strtolower($arConfig[$i]['type']);
				if ( empty($type) && is_numeric($coluna) ){ 
					$type = self::TYPENUMERIC;
				}elseif ( empty($type) ){ 
					$type = self::TYPESTRING;
				}
				$coluna = trim($coluna);
				// caso tenha sido definido o totalizador
				if ( $this->config['totalLinha'] ){
					// totaliza todos os campos numéricos
					self::calculaTotalLinha($indColuna, $coluna, $type);
				}
				//Este controle garante que os campos do corpo da lista não ultrapassem a quantidade de campos do cabeçalho
				if ($i >= $totalColuna)	continue;
//				if ($i >= ($this->totalColuna + $this->acaoAltTotalColuna))	continue;

				// Faz o tratamento do valor pelo tipo
				$coluna = self::trataType($coluna, $type);
				// Caso tenha html configurado faz a substituição
				if ( $arConfig[$i]['html'] && !$arConfig[$i]['php']){
					$coluna = self::carregaHTML($arConfig[$i], $linha);
				} elseif($arConfig[$i]['html'] && $arConfig[$i]['php']) {
					foreach($linha as $indCol => $valCol){
						$indHTML[] = '{' . $indCol . '}';
						$valHTML[] = self::trataType( $valCol, $type );
					}
					
					$expressao = '$boolean = ' . str_replace($indHTML, $valHTML, $arConfig[$i]['php']['expressao']) . ';';
					eval($expressao);
					
					$condicaoTrue = str_replace($indHTML, $valHTML, $arConfig[$i]['php']['true']);
					$condicaoFalse = str_replace($indHTML, $valHTML, $arConfig[$i]['php']['false']);
					
					if($boolean){
						$coluna = $condicaoTrue;
					} else {
						$coluna = $condicaoFalse;
					}
					unset($boolean,$expressao,$indHTML,$valHTML);
					
				}
				
				// Cria a célula e seta seu valor 				
				$cell = $row->addCell( $coluna );
				// configura a célula pelo "type"
				if ( ($type == self::TYPENUMERIC || $type == self::TYPENUMERICNATIVO || $type == self::TYPEMONEY) ){
					$cell->align = 'right';
					$cell->style = 'color:#0066CC;';
				}elseif ( $type == self::TYPEDATE ){
					$cell->align = 'center';
				}elseif( $type == self::TYPETIME ){
					$cell->align = 'right';
				}else{
					$cell->align = 'left';
				}
				// Atribuição do alinhamento, vindo do config.
				if ( $arConfig[$i]['align'] ){
					$cell->align = $arConfig[$i]['align'];						
				}
				
				// Atribuição do style, vindo do config.
				$style 		  = $cell->style;
				$style 		 .= $arConfig[$i]['style'];
				$cell->style = $style;
				
				$i++;
			endforeach;
			self::montaSubLinha($linha);
			$contLinha++;
		endforeach;
		self::montaRodape();
		if($this->boPaginacao){
			self::montaPaginacao();
		}
	}
	
	private function montaSubLinha($linha){
		if ($this->config['subLinha'] !== false){
			$id    = self::carregaHTML($this->config['subLinha'], $linha); 
			$table = $this->table;
			$row   = $table->addRow();
			$row->style = 'display:none;';
			$cell  = $row->addCell('');
			$cell->id = "subLinha_" . $id; 
			$cell->colspan = ($this->totalColuna);
//			$cell->colspan = ($this->totalColuna + $this->acaoAltTotalColuna);
		}
	}
	
	private function montaRodape(){
		if ( $this->config['totalLinha'] ) 	  self::montaTotalLinha();
		if ( $this->config['totalRegistro'] ) self::montaTotalRegistro();
	}
	
	private function montaTotalLinha(){
		$arColuna  = $this->arCorpo['dados'][0];
		$arConfig  = $this->arCorpo['config'];
		$table 	   = $this->table;
		$row  	   = $table->addRow();
		$row->bgcolor = '#E3E3E3';
		// Primeira célula
		$cell  	   = $row->addCell('Totais:');
		
		$totalColuna = $this->totalColuna;
		
		$i = 0;
		if ( !$this->acao ){
			unset($arColuna[key($arColuna)]);
			$i++;
		}else{
			$totalColuna += ($this->acao && $this->arCabecalho) ? -1 : 0; 
//			$totalColuna--;
		}

		foreach ( $arColuna as $indCol => $valCol ):
			if ( $i >= ($totalColuna) ) break;
		
			// Atribui o valor total da coluna
			$valCol = trim($this->arCorpoTotal[$indCol]);
			
			// Pula a primeira coluna do array, pois ela será substituída pelo total			
//			if ( $i == 0 ){ 
//				$i++;
//				continue;
//			}
			// define tipo de dado
			$type = strtolower($arConfig[$i]['type']);
			if ( empty($type) && is_numeric($valCol) ){ 
				$type = self::TYPENUMERIC;
			}elseif ( empty($type) ){ 
				$type = self::TYPESTRING;
			}
			// Somente campo do tipo numérico, ou seja, que permite somatória
			if ( ((is_numeric( $valCol ) || $arConfig[$i]['html']) 
				  && ($type == self::TYPENUMERIC || $type == self::TYPENUMERICNATIVO || $type == self::TYPEMONEY) 
				  || $type == self::TYPETIME) 
				 && $arConfig[$i]['totalLinha'] !== false ){

				// Faz o tratamento do valor pelo tipo
				$valCol = self::trataType($valCol, $type);
				// Caso tenha html configurado faz a substituição
				if ( $arConfig[$i]['html'] ){
					$valCol = self::carregaHTML($arConfig[$i], $this->arCorpoTotal);
				}	
			}else{
				$valCol = '-';
			}
			// Adiciona Célula
			$cell = $row->addCell( $valCol );
			$cell->nowrap = 'nowrap';
			
			if ( $valCol !== '-' ){
				$cell->align = 'right';

				if ( $type !== self::TYPETIME )
					$cell->style = 'color:#0066CC;';
				// Atribuição do alinhamento, vindo do config.
				if ( $arConfig[$i]['align'] ){
					$cell->align = $arConfig[$i]['align'];						
				}
			}
			$i++;
		endforeach;
	}
	
	private function montaTotalRegistro(){
		$totalColuna = $this->totalColuna;
		// Caso possua "Ação" e NÃO possua "Cabeçalho" aumenta 1 do total de colunas.
		$totalColuna += ($this->acao && empty($this->arCabecalho)) ? 1 : 0; 
		
		$qtdCol = $totalColuna;
		$qtdReg = count($this->arCorpo['dados']);
		$table  = $this->table;
		$row 	= $table->addRow();
		$cell 	= $row->addCell("Total de Registros: $qtdReg");
		$cell->colspan = $qtdCol; 
		$cell->nowrap  = 'nowrap'; 
		$cell->style   = 'border-top: 2px solid #000000; font-weight: bold;'; 
	}
	
	private function carregaHTML($config, Array $linha){
		if ( is_array($config) ){
			$html = $config['html'];
			$type = strtolower($config['type']);
		}else{
			$html = $config;
		}	
		
		foreach($linha as $indCol => $valCol){
			$indHTML[] = '{' . $indCol . '}';
			$valHTML[] = self::trataType( $valCol, $type );
		}
		return str_replace($indHTML, $valHTML, $html);
	}
	
	private function trataType($valor, $type = null){
//		$valor = trim($valor);
		
		/*if ( $type == self::TYPENUMERIC && is_numeric(trim($valor)) ){
			$valor = number_format($valor, 2, ',', '.');
		}elseif ( $type == self::TYPEMONEY && is_numeric(trim($valor)) ){*/
		if ( $type == self::TYPENUMERIC && is_numeric($valor) ){
			if ( is_string($valor) && strpos($valor, '.') !== false ){
				$valor = (real) $valor;
			}
			$valor = ( $valor == 0 || is_float($valor) === false ) ? (integer) $valor : number_format($valor, 2, ',', '.');
		}elseif ( $type == self::TYPEMONEY && is_numeric($valor) ){
			$valor = number_format($valor, 2, ',', '.');
		}elseif ( $type == self::TYPETIME && ( strpos($valor, ':') !== false || is_numeric($valor) ) ){
			// Transforma o valor de TIME em SEGUNDO
			$seconds = self::transformaTimeSegundo( $valor );
			// Transforma o valor de SEGUNDO em TIME	
			$valor	 = self::transformaSegundoTime( $seconds ); 				
		}	
		
		return $valor;
	}

	private function transformaTimeSegundo( $time ){
		$seconds = 0;
		if ( $time ){
			list( $g, $i, $s ) = explode( ':', $time );
		    $seconds += $g * 3600;
		    $seconds += $i * 60;
		    $seconds += $s;	
		}
		
	    return $seconds;
	}
	
	private function transformaSegundoTime( $seconds ){
		if( $seconds ) {
					
			$hours = floor( $seconds / 3600 );
			$seconds -= $hours * 3600;
			$minutes = floor( $seconds / 60 );
			$seconds -= $minutes * 60;
			
			$time = sprintf("%02d", $hours) . ":" . sprintf("%02d", $minutes);
		} else {
			$time = "00:00";
		}	

		return $time;
	}
	
	private function calculaTotalLinha($indice, $coluna, $type = null){
		
		if ( is_numeric($coluna) && $type != self::TYPETIME ){
			$this->arCorpoTotal[$indice] += $coluna;
		}elseif ( $type == self::TYPETIME && ( strpos($coluna, ':') !== false || is_numeric($coluna) ) ){
			$segHistorico = self::transformaTimeSegundo( $this->arCorpoTotal[$indice] );
			$segNovo	  = self::transformaTimeSegundo( $coluna );
			$this->arCorpoTotal[$indice] = self::transformaSegundoTime( $segHistorico + $segNovo );
		}
	}
	
	private function atribuiNivelCabecalho(Array $arCabecalho = null, $row){
		if ( !is_null($arCabecalho) ){
			$this->rowspan 		= 1;
			$this->rowspanTotal = $this->rowspanTotal < $row ? $row : $this->rowspanTotal;
			
			foreach($arCabecalho as $indice => $cabecalho){
				$txtCabecalho = is_array($cabecalho) ? $indice : $cabecalho;
				$this->cell[$txtCabecalho]['row']     = empty($this->cell[$txtCabecalho]['row'])     
														? $row 
														: $this->cell[$txtCabecalho]['row'];
				$this->cell[$txtCabecalho]['rowspan'] = empty($this->cell[$txtCabecalho]['rowspan']) 
														? $this->rowspan 
														: $this->cell[$txtCabecalho]['rowspan'];
				
				if ( is_array($cabecalho) ){
//					$this->rowspanTotal++;
					$this->cell[$txtCabecalho]['colspan'] = self::calculaNivelCabecalho( $cabecalho );
					$this->cell[$txtCabecalho]['rowspan'] = $this->rowspan;


					self::atribuiNivelCabecalho( $cabecalho, ($row + 1) );
				}
			}
		}	
	} 
	
	private function calculaNivelCabecalho(Array $arCabecalho){
		$this->rowspan++;
		$colspan = count( $arCabecalho );
		foreach($arCabecalho as $indice => $cabecalho){
			$txtCabecalho = is_array($cabecalho) ? $indice : $cabecalho;
			if ( is_array($cabecalho) ){
				$colspanRetorno += self::calculaNivelCabecalho( $cabecalho, ($row+1) );
			}else{
				$colspanRetorno++;
			}
		}
		$colspan = ($colspan > $colspanRetorno ? $colspan : $colspanRetorno);
		
		return $colspan;
	}
}
?>