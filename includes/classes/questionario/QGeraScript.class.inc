<?php
class QGeraScript{
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
    private $db;
    static $i;
                
    /*
	 * Monta a Tela de Geraчуo de Script do Questionсrio
	 *
	 * include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
	 * include_once APPRAIZ . "includes/classes/questionario/QGeraScript.class.inc";
	 * include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	 * 
	 * $obGera = new QGeraScript();
	 * echo $obGera->montaArvoreQ();
	 *  
	 * Desenvolvedor: Victor Benzi
	 * 
	 */
    
	public function __construct(Array $param = null){
		$this->s  = new Sistema();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo();
		$this->p  = new QPergunta();
		$this->t  = new Tabela();
		$this->gc = new Grupocoluna();
		$this->gl = new Grupolinha();
		$this->c  = new Coluna();
		$this->l  = new Linha();
		$this->it = new QItempergunta();
		$this->r  = new QResposta();
 
		$this->perid = $param['perid'];
		$this->queid = array( "queid" => $_REQUEST['queid'] );
		
		global $db;
		$this->db = $db;		
	}
                        
	function montaArvoreQ(){
		$obPai = $this->q->pegaUmObjeto( $this->queid );
		
	//	pg_query( $db->link, 'begin transaction;');
		$arrDados = array(	
			"SQL"   => "INSERT INTO questionario.questionario ( sisid, quetitulo, quedescricao, quedtinicio, quedtfim ) VALUES (resultSisid, '".utf8_encode($obPai->quetitulo)."', '".utf8_encode($obPai->quedescricao)."', '$obPai->quedtinicio', '$obPai->quedtfim') RETURNING queid",
			"tipo"  => "questionario",
			"var"   => "queid",
			"itens" => array());
			
			// Busca Perguntas associadas
			$arrP = self::montaArvoreP($obPai);
			if( is_array( $arrP )){
				array_push( $arrDados['itens'], $arrP );
			}
			
			// Busca Grupo Pergunta associadas
			$arrG = self::montaArvoreG($obPai);
			if( is_array( $arrG )){
				array_push( $arrDados['itens'], $arrG );
			}

		$this->arrDados = $arrDados;
		//ver($arrDados);
	//	self::percorreArray( $arrDados );
		self::exportarArquivo();
	}
	
	function montaArvoreG( $obPai ){

		// Filtra Grupos vinculadas
		$arrObGrupoPergunta = (array) $this->g->listaObjeto(array( $obPai ), array("*"));
		$i = 0;
		$gru = '';
		if( simec_get_class($obPai) == 'QGrupo' ){
			$gru = 'gru_';
		}
		foreach ($arrObGrupoPergunta as $obGrupoPergunta){
			$grupos[] = array(	
				"SQL"   => "INSERT INTO questionario.grupopergunta( ".$gru."varPai, grptitulo, grpstatus, grpordem, grpajuda) VALUES ( resultPai, '".utf8_encode($obGrupoPergunta->grptitulo)."', '$obGrupoPergunta->grpstatus', $obGrupoPergunta->grpordem, '".utf8_encode($obGrupoPergunta->grpajuda)."') RETURNING grpid",
				"tipo"  => "grupo",
				"var"   => "grpid",
				"itens" => array());
						
			// Busca Perguntas associadas
			$arrG = self::montaArvoreP($obGrupoPergunta);
			if( is_array( $arrG )){
				array_push( $grupos[$i]['itens'], $arrG );
			}
			
			// Busca Grupo Pergunta associadas
			$arrP = self::montaArvoreG($obGrupoPergunta);
			if( is_array( $arrP )){
				array_push( $grupos[$i]['itens'], $arrP );
			}
			$i++;
		}
		return $grupos;
	}
	
	function montaArvoreP( $obPai ){
		// Filtra perguntas vinculadas
		$arrObPergunta = (array) $this->p->listaObjeto(array( $obPai ), array( '*' ));
		$i = 0;
		foreach ($arrObPergunta as $obPergunta){
			$perguntas[] = array(
				"SQL"   => "INSERT INTO questionario.pergunta( varPai, perordem, pertitulo, perajuda, pertipo, permascara, perobrigatorio, permultipla) VALUES ( resultPai, $obPergunta->perordem, '".utf8_encode($obPergunta->pertitulo)."', '".utf8_encode($obPergunta->perajuda)."', '$obPergunta->pertipo', '$obPergunta->permascara', '$obPergunta->perobrigatorio', '$obPergunta->permultipla') RETURNING perid",
				"tipo"  => "pergunta",
				"var"   => "perid",
				"itens" => array());
			
			// Busca Campo Externo (Tabela)
			$arrE = self::montaArvoreExterno( $obPergunta );
			if( is_array( $arrE ) ){
				array_push( $perguntas[$i]['itens'], $arrE );
			}
			
			// Busca Itens associados
			$arrI = self::montaArvoreI( $obPergunta );
			if( is_array( $arrI )){
				array_push( $perguntas[$i]['itens'], $arrI );
			}
			$i++;
		}
		return $perguntas;
	}
	
	function montaArvoreExterno( $obPai ){
		$i = 0;
		$arrObExtPergunta = (array) $this->t->listaObjeto( array( $obPai ) );
		
		foreach( $arrObExtPergunta as $obExtPergunta ){
			$tblpercent 	  = $obExtPergunta->tblpercent ? $obExtPergunta->tblpercent : 'NULL';
			$tbltotalizadorTx = $obExtPergunta->tbltotalizador ? ', tbltotalizador' : '';
			$tbltotalizador   = $obExtPergunta->tbltotalizador ? ", '".utf8_encode($obExtPergunta->tbltotalizador)."'" : '';
			$tabelas[] = array(
				"SQL" => "INSERT INTO questionario.tabela(perid, tbldescricao, tbllabelcolunas, tbllabellinhas, tbltipo, tblpercent ".$tbltotalizadorTx.") VALUES ( resultPai, '".utf8_encode($obExtPergunta->tbldescricao)."', '".utf8_encode($obExtPergunta->tbllabelcolunas)."', '".utf8_encode($obExtPergunta->tbllabellinhas)."', '".utf8_encode($obExtPergunta->tbltipo)."', ".$tblpercent." ".$tbltotalizador.") RETURNING tblid",
				"tipo" => "tabela",
				"var"  => "tblid",
				"itens" => array());
		
			// Verifica se tem Grupo de Colunas
			$arrGC = self::montaArvoreExternoGrupoColuna( $obExtPergunta );
			if( is_array( $arrGC ) ){
				array_push( $tabelas[$i]['itens'], $arrGC );
			}
			
			//Verifica se tem Grupo de Linhas
			$arrGL = self::montaArvoreExternoGrupoLinha( $obExtPergunta );
			if( is_array( $arrGL ) ){
				array_push( $tabelas[$i]['itens'], $arrGL );
			}
			
			$i++;
		}
		
		return $tabelas;
	}
	
	function montaArvoreExternoGrupoColuna( $obPai ){

		$gru = '';
		$i = 0;
		if( simec_get_class($obPai) == 'Grupocoluna' ){
			$gru = 'gru_';
		}
		
		$arrObExtGrupoColuna = (array) $this->gc->listaObjeto( array( $obPai ) );
		//ver($arrObExtGrupoColuna);
		
		foreach( $arrObExtGrupoColuna as $obExtGrupoColuna ){
			$grupocoluna[] = array(
				"SQL" => "INSERT INTO questionario.grupocoluna(".$gru."varPai, grcdsc) VALUES ( resultPai, '".utf8_encode($obExtGrupoColuna->grcdsc)."') RETURNING grcid",
				"tipo" => "grupocoluna",
				"var"  => "grcid",
				"itens" => array());
		
			// Verifica se tem Grupo de Colunas filho dele
			$arrGC = self::montaArvoreExternoGrupoColuna( $obExtGrupoColuna );
			if( is_array( $arrGC )){
				array_push( $grupocoluna[$i]['itens'], $arrGC );
			}
			
			// Verifica se tem Grupo de Linhas
			$arrC = self::montaArvoreExternoColuna( $obExtGrupoColuna );
			if( is_array( $arrC ) ){
				array_push( $grupocoluna[$i]['itens'], $arrC );
			}
			
			$i++;
		}
		
		return $grupocoluna;
	}
	
	function montaArvoreExternoColuna( $obPai ){

		$arrObExtColuna = (array) $this->c->listaObjeto( array( $obPai ) );
		
		foreach( $arrObExtColuna as $obExtColuna ){
			$coltipoTx 		    = $obExtColuna->coltipo ? ', coltipo' : '';
			$coltipo 		    = $obExtColuna->coltipo ? ", '".utf8_encode($obExtColuna->coltipo)."'" : '';
			$collegendaTx 	    = $obExtColuna->collegenda ? ', collegenda' : '';
			$collegenda 	    = $obExtColuna->collegenda ? ", '".utf8_encode($obExtColuna->collegenda)."'" : '';
			$colmascaraTx 	    = $obExtColuna->colmascara ? ', colmascara' : '';
			$colmascara 	    = $obExtColuna->colmascara ? ", '".utf8_encode($obExtColuna->colmascara)."'" : '';
			$coltamanhomaximoTx = $obExtColuna->coltamanhomaximo ? ', coltamanhomaximo' : '';
			$coltamanhomaximo   = $obExtColuna->coltamanhomaximo ? ", ".$obExtColuna->coltamanhomaximo."" : '';
			$coluna[] = array(
				"SQL" => "INSERT INTO questionario.coluna(grcid, itmid ".$coltipoTx.$collegendaTx.$colmascaraTx.$coltamanhomaximoTx.") VALUES ( resultPai, varItem ".$coltipo.$collegenda.$colmascara.$coltamanhomaximo." ) RETURNING grcid",
				"tipo" => "coluna",
				"itmdsc"  => utf8_encode($this->c->pegaItem($obExtColuna->itmid)),
				"var"  => "colid",
				"itens" => array());
		}
		
		return $coluna;
	}
	
	function montaArvoreExternoGrupoLinha( $obPai ){

		$gru = '';
		$i = 0;
		if( simec_get_class($obPai) == 'Grupolinha' ){
			$gru = 'gru_';
		}
		
		$arrObExtGrupoLinha = (array) $this->gl->listaObjeto( array( $obPai ) );
		//ver($arrObExtGrupoColuna);
		
		foreach( $arrObExtGrupoLinha as $obExtGrupoLinha ){
			$grupolinha[] = array(
				"SQL" => "INSERT INTO questionario.grupolinha(".$gru."varPai, grldsc) VALUES ( resultPai, '".utf8_encode($obExtGrupoLinha->grldsc)."') RETURNING grlid",
				"tipo" => "grupolinha",
				"var"  => "grlid",
				"itens" => array());
		
			// Verifica se tem Grupo de Colunas filho dele
			$arrGL = self::montaArvoreExternoGrupoLinha( $obExtGrupoLinha );
			if( is_array( $arrGL )){
				array_push( $grupolinha[$i]['itens'], $arrGL );
			}
			
			// Verifica se tem Grupo de Linhas
			$arrC = self::montaArvoreExternoLinha( $obExtGrupoLinha );
			if( is_array( $arrC ) ){
				array_push( $grupolinha[$i]['itens'], $arrC );
			}
			
			$i++;
		}
		
		return $grupolinha;
	}
	
	function montaArvoreExternoLinha( $obPai ){

		$arrObExtLinha = (array) $this->l->listaObjeto( array( $obPai ) );
		
		foreach( $arrObExtLinha as $obExtLinha ){
			$lindscTx = $obExtLinha->lindsc ? ', lindsc' : '';
			$lindsc = $obExtLinha->lindsc ? ", '".utf8_encode($obExtLinha->lindsc)."'" : '';
			$linha[] = array(
				"SQL" => "INSERT INTO questionario.linha(grlid, itmid ".$lindscTx.") VALUES ( resultPai, varItem ".$lindsc." ) RETURNING linid",
				"tipo" => "linha",
				"itmdsc"  => utf8_encode($this->c->pegaItem($obExtLinha->itmid)),
				"var"  => "linid",
				"itens" => array());
		}
		
		return $linha;
	}
	
	function montaArvoreI( $obPai ){
		
		$where[0] = $obPai;

		// Filtra perguntas vinculadas
		$arrObItPergunta = (array) $this->it->listaObjeto($where, array("itpid, itptitulo"));
		$i = 0;
		foreach ($arrObItPergunta as $obItPergunta){
			$itens[] = array(
				"SQL" => "INSERT INTO questionario.itempergunta(perid, itptitulo) VALUES ( resultPai, '".utf8_encode($obItPergunta->itptitulo)."') RETURNING itpid",
				"tipo" => "item",
				"var"  => "itpid",
				"itens" => array()); 				
			
			// Busca Perguntas associadas
			$arrP = self::montaArvoreP($obItPergunta);
			if( is_array( $arrP )){
				array_push( $itens[$i]['itens'], $arrP );
			}

			// Busca Grupo Pergunta associadas
			$arrG = self::montaArvoreG($obItPergunta);
			if( is_array( $arrG )){
				array_push( $itens[$i]['itens'], $arrG );
			}
			$i++;
		}
		return $itens;
	}
	
	function percorreArray( $arrDados, $sisid = null, $pai = null, $resultPai = null ){
		//ver( $arrDados, d );
		if( $arrDados['SQL'] && strlen( $arrDados['SQL'] ) > 5 ){
			$sql = str_replace('resultSisid', $sisid, utf8_decode($arrDados['SQL']));
			$sql = str_replace('varPai', $pai, $sql);
			if( $arrDados['tipo'] == 'coluna' || $arrDados['tipo'] == 'linha' ){
				$item = $this->db->pegaUm( "SELECT itmid FROM questionario.item WHERE itmdsc ilike '".trim(utf8_decode($arrDados['itmdsc']))."'" );
				if( !$item || $item == '' ){
					$item = $this->db->pegaUm( "INSERT INTO questionario.item(itmdsc) VALUES ('".trim(utf8_decode($arrDados['itmdsc']))."') RETURNING itmid" );
				}
				$sql = str_replace('varItem', $item, $sql);
			}
			$sql = str_replace('resultPai', $resultPai, $sql );
			$resultPai = $this->db->pegaUm( $sql . ' -- '.trim(utf8_decode($arrDados['itmdsc'])) );
		}
		if( $arrDados['itens'] ){
			$pai = $arrDados['var'];
			if( is_array( $arrDados['itens'] ) ){
				foreach( $arrDados['itens'] as $arr ){
					self::percorreArray( $arr, $sisid, $pai, $resultPai );
				}
			}
		} else {
			if( is_array($arrDados) ){
				foreach( $arrDados as $arr ){
					self::percorreArray( $arr, $sisid, $pai, $resultPai );
				}
			}
		}
		//$this->db->commit();
		return 1;
	}
	
	function exportarArquivo(){
		
//		self::geraScript();
//		dbg($this->script, 1);
		ob_clean();

		// Headers for an download:
//		ver($this->arrDados,d);
//		header('Content-Type: application/octet-stream');
//		header('content-type: application/json;charset=iso-8859-1');
		header('Content-Disposition: attachment; filename="Script.txt"');
		header('Content-Transfer-Encoding: binary');
		
		echo simec_json_encode( $this->arrDados );
		exit;
	}
	
	public function extrairArquivo($arquivo, $sisid){
//		phpinfo();
//		die;
		$script		  = file_get_contents($arquivo);
		$this->script = json_decode( $script, true );
		if( is_array( $this->script ) ){
			$exec = self::percorreArray( $this->script, $sisid );
		} else {
			$exec == 0;
		}
		if( $exec == 1 ){
			$this->db->commit();
			return 'Questionсrio gerado com sucesso!';
		} else {
			$this->db->rollback();
			return 'Erro encontrado!';
		}
	}
	
}
?>