<?php
/**
 * 
 * Componente de montagem de grid. Monta uma grid que ordena e pagina.
 * Depende das classes: Ordenador e Paginador. 
 * @author Ráyner Lima <rayner.lima@squadra.com.br>
 *
 */
class Grid {

	private $ordenador;
	private $paginador;
	private $strNomeGrid;
	private $strUrlAjax;

	public function __construct( $arrCamposOrdenacao, $strUrlAjax, $strNomeGrid,
	$intQtdeTotalRegistros, $intQtdePorPagina = Paginador::LIMIT_PADRAO ) {

		$this->setNomeGrid($strNomeGrid);
		$this->setUrlAjax($strUrlAjax);

		$ordenador = new Ordenador();
		$ordenador->setCamposOrdenacao($arrCamposOrdenacao);
		$ordenador->setNomeOrdenador($strNomeGrid);
		$ordenador->setUrlAjax($strUrlAjax);
		$this->ordenador = $ordenador;

		$paginador = new Paginador();
		$paginador->setNomePaginador($strNomeGrid);
		$paginador->setQtdeTotalRegistros($intQtdeTotalRegistros);
		$paginador->setQtdePorPagina($intQtdePorPagina);
		$paginador->setUrlAjax($strUrlAjax);
		$this->paginador = $paginador;
	}

	/**
	 *
	 * Informa um nome para a grid. O nome será utilizado para identificar com atributos <code>id</code> e <code>class</code>.
	 * @param str $strNomeGrid
	 */
	public function setNomeGrid( $strNomeGrid ){
		$this->strNomeGrid = $strNomeGrid;
	}

	/**
	 *
	 * Retorna um nome para a grid. O nome será utilizado para identificar com atributos <code>id</code> e <code>class</code>.
	 */
	public function getNomeGrid() {
		return $this->strNomeGrid;
	}

	/**
	 *
	 * Informa a url de uma action ajax onde os registros de uma determinada página serão apresentados.
	 * @param unknown_type $strUrlAjax
	 */
	public function setUrlAjax( $strUrlAjax ){
		$this->strUrlAjax = $strUrlAjax;
	}

	/**
	 *
	 * Retorna a url de uma action ajax onde os registros de uma determinada página serão apresentados.
	 */
	public function getUrlAjax(){
		return $this->strUrlAjax;
	}

	/**
	 *
	 * Retorna uma string html que será impresso na tela.
	 */
	public function montaGrid(){
		$nomeGrid = $this->getNomeGrid();
		$elemento  = "<div class=\"grid $nomeGrid\">";
		$elemento .= 	"<table class=\"listagem $nomeGrid\">";
		$elemento .= 		$this->ordenador->montaOrdenador();
		$elemento .=		"<tbody id=\"grid\" class=\"tbody-$nomeGrid\">";
		$elemento .=		"</tbody>";
		$elemento .= 	"</table>";

		$elemento .= 	"<table class=\"listagem paginador-$nomeGrid\">";
		$elemento .=		$this->paginador->montaPaginador();
		$elemento .= 	"</table>";
		$elemento .= "</div>";

		$elemento .= "<script type=\"text/javascript\">";
		$elemento .= 	$this->registraJS();
		$elemento .= "</script>";
		return $elemento;
	}
	
	public static function atualizaPaginador( $listaObjetos ){
		$paginador = new Paginador();
		$paginador->setQtdeTotalRegistros( count($listaObjetos) );
		return $paginador->montaPaginador();
	}

	/**
	 *
	 * Retorna uma string javascript que será impresso na tela.
	 * Server para registrar os eventos de ordenação e paginação
	 */
	public function registraJS(){

		$urlAjax = $this->getUrlAjax();
		$qtdePagina = $this->paginador->getQtdePorPagina();

		$elementoJS = "
		var OrdenadorPaginador = {
	
			init : function(){
				$('.ordenador').click(function (event){
					event.preventDefault();
					var ordem = $(this).attr('id').substr($(this).attr('id').indexOf('-')+1);
					var campo = $(this).attr('id').substr(0, $(this).attr('id').indexOf('-'));
					var urlLink = $(this).attr('href');
					
					var limit =  $('#paginador a.ativo').attr('id').substr($('#paginador a.ativo').attr('id').indexOf('-')+1);
					var offset = $('#paginador a.ativo').attr('id').substr(0, $('#paginador a.ativo').attr('id').indexOf('-'));
					
					OrdenadorPaginador.recuperaItens(urlLink, campo, ordem, limit, offset);
					
					if (ordem == 'asc') {
						$(this).attr('id', campo+'-desc');
					} else {
						$(this).attr('id', campo+'-asc');
					}
				});
				
				$('a.paginador').click(function(event){
					
					event.preventDefault();
					
					if (!$(this).hasClass('unico')){
						
						$('a.paginador').removeClass('ativo');
						$(this).addClass('ativo');
						
						var urlLink = $(this).attr('href');
						var ordem = 'asc';
						var campo = $('#ordenador a.ativo').attr('id').substr(0, $('#ordenador a.ativo').attr('id').indexOf('-'));
						var limit =  $(this).attr('id').substr($(this).attr('id').indexOf('-')+1);
						var offset = $(this).attr('id').substr(0, $(this).attr('id').indexOf('-'));
						
						OrdenadorPaginador.recuperaItens(urlLink, campo, ordem, limit, offset);
						
					}
				});
				
				OrdenadorPaginador.recuperarTodosItens('$urlAjax', $qtdePagina);
				
			},
			
			recuperaItens : function (urlLink, campo, ordem, limit, offset){
				$.ajax({
					beforeSend: function(){
						$('#aguarde').css('visibility', 'visible');
						$('#aguarde').show();
					},
					type: 'post',
					url: urlLink,
					cache: false,
					dataType: 'html',
					data: $('form').serialize() + '&campo=' + campo + '&ordem=' + ordem + '&limit=' + limit + '&offset=' + offset,
					success: function(data){
						//console.log(data);
						$('#grid').empty();
						$('#grid').html(data);
					},
					complete: function(){
						$('#aguarde').css('visibility', 'hidden');
						$('#aguarde').hide();
					}
				});
			},
			
			recuperarTodosItens : function (urlLink, limit) {
				OrdenadorPaginador.recuperaItens(urlLink, '', '', limit, 0);
			},
			
			atualizaPaginador : function( qtdeRegistros, paginas ) {
				
			}
		}
		
		OrdenadorPaginador.init();
		";

		return $elementoJS;
	}
}