<?php

require_once( 'tipocredito_funcoes.inc' );

switch ( $_REQUEST['acao'] )
{
	// inserir ou editar
	case 'E':
		require 'tipocredito_editar.inc';
		break;
	// remover
	// apуs a remoзгo a listagem й acionada
	case 'R':
		require 'tipocredito_remover.inc';
	// listar
	case 'L':
		require 'tipocredito_listar.inc';
		break;
	
}
